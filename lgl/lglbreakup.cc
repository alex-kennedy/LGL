//
//  Copyright (c) 2002 Alex Adai, All Rights Reserved.
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License as
//  published by the Free Software Foundation; either version 2 of
//  the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston,
//  MA 02111-1307 USA
//
//
/////////////////////////////////////////////////////////////////////////

#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <vector>

#include "boost/graph/connected_components.hpp"
#include "lgl/lib/configs.h"
#include "lgl/lib/graph.h"
#include "lgl/lib/io.h"

using namespace lgl::lib;

/////////////////////////////////////////////////////////////////////////

const char* doutputdir = "/tmp/lgl_temp";
const bool defaultWrite = true;
const prec_t cutoff = 1e30;
const bool defaultDoesWriteLgl = true;

/////////////////////////////////////////////////////////////////////////

typedef std::set<Graph<FloatType>::vertex_descriptor> ProcessList;

/////////////////////////////////////////////////////////////////////////

void displayUsage(char** argv);
int connected_sets(const Graph<FloatType>& g, WriteList& writelist);
void addAllEdgesFromVertices(const Graph<FloatType>& g,
                             std::vector<int>& components,
                             ProcessList& process);

/////////////////////////////////////////////////////////////////////////

bool set_size_sort(const std::vector<int>& v1, const std::vector<int>& v2) {
  return v1.size() > v2.size();
}

/////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
  // There is just one input arg,
  // a file name with all the connections.
  if (argc == 1) {
    displayUsage(argv);
  }

  // Defaults
  const char* outputdir = doutputdir;
  bool doesWrite = defaultWrite;
  bool writeNewLGL = defaultDoesWriteLgl;
  bool useMST = false;
  prec_t cut = cutoff;
  int b;

  int optch;
  while ((optch = getopt(argc, argv, "d:w:c:sm")) != -1) {
    switch (optch) {
      case 'd':
        outputdir = strdup(optarg);
        break;
      case 'w':
        b = atoi(optarg);
        if (b) {
          doesWrite = true;
        } else {
          doesWrite = false;
        }
        break;
      case 'c':
        cut = (prec_t)atof(optarg);
        break;
      case 's':
        writeNewLGL = !writeNewLGL;
        break;
      case 'm':
        useMST = true;
        break;
      default:
        std::cerr << "Bad Option. Exiting.";
        exit(EXIT_FAILURE);
    }
  }

  char* infile = strdup(argv[optind]);

  std::cerr << "Loading " << infile << "..." << std::flush;
  Graph<FloatType> G;
  readLGL(G, infile, cut);
  std::cerr << "Done." << std::endl;

  int vertexCount = G.vertexCount();
  int edgeCount = G.edgeCount();
  int emin = std::max<int>((int).5 * edgeCount, 25000);

  std::cerr << vertexCount << " : Total Vertex Count\n"
            << edgeCount << " : Total Edge Count\n"
            << "Determining connected sets..." << std::flush;

  if (vertexCount == 0) {
    std::cerr << "None.\n";
    return EXIT_SUCCESS;
  }

  if (useMST) {
    std::cerr << "Using MSTs..." << std::flush;
    if (!G.hasWeights()) {
      std::cerr << "Tree doesn't have weights. Exiting.\n";
      exit(EXIT_FAILURE);
    }
    Graph<FloatType> mst;
    setMSTFromGraph(G, mst);
    G.clear();
    G = mst;
    vertexCount = G.vertexCount();
    edgeCount = G.edgeCount();
  }

  // Finds connected sets without making any recursive calls.
  // ( Protects stacks from very large graphs )
  WriteList writelist;
  int num = connected_sets(G, writelist);
  std::cerr << "\nFound " << num << " connected sets." << std::endl;

  if (writeNewLGL) {
    std::string keyfile(outputdir);
    keyfile += "_new_lgl.lgl";
    writeLGL(G, keyfile.c_str());
  }

  std::string keyfile(outputdir);
  keyfile += "_vertex_file_match";
  std::ofstream fileSetMatch(keyfile.c_str());
  if (!fileSetMatch) {
    std::cerr << "Open of " << keyfile << " failed" << std::endl;
    exit(EXIT_FAILURE);
  }

  int eout = 0, sets = 0;
  // Now to make a call for each connected set.
  for (int currentSet = 0; currentSet < num; ++currentSet) {
    char outfile[256];
    sprintf(outfile, "%s/%d.lgl", outputdir, currentSet);
    std::cerr << "Writing " << outfile << std::endl;
    eout += writeCurrentLGL(G, outfile, sets, writelist, doesWrite, cut,
                            fileSetMatch);
    ++sets;
    if (eout > emin) {
      std::cerr << "Remapping\n";
      remap(G);
      emin = std::max<int>((int).5 * (edgeCount - eout), 25000);
      edgeCount -= eout;
      eout = 0;
      connected_sets(G, writelist);
      sets = 0;
    }
  }

  return EXIT_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////

int connected_sets(const Graph<FloatType>& g, WriteList& writelist) {
  Graph<FloatType>::vertex_descriptor vother;
  Graph<FloatType>::vertex_iterator v1, v2;
  Graph<FloatType>::out_edge_iterator oe, oend;
  ProcessList process;
  std::vector<int> components(num_vertices(g.boostGraph()), -1);

  int currentComponent = 0;
  // int vertexCounter = 0;
  // cerr << "Vertex: " << std::setw(8) << vertexCounter;
  for (std::tie(v1, v2) = vertices(g.boostGraph()); v1 != v2; ++v1) {
    // cerr << "\b\b\b\b\b\b\b\b" << std::setw(8) <<  ++vertexCounter << flush;
    if (components[*v1] >= 0) {
      continue;
    }
    components[*v1] = currentComponent;
    for (std::tie(oe, oend) = out_edges(*v1, g.boostGraph()); oe != oend;
         ++oe) {
      vother = target(*oe, g.boostGraph());
      components[vother] = currentComponent;
      process.insert(vother);
    }
    while (!process.empty()) addAllEdgesFromVertices(g, components, process);
    ++currentComponent;
  }

  // Makes the output alot quicker to figure out what gets
  // written ahead of time
  writelist.clear();
  writelist.resize(currentComponent);
  for (std::vector<int>::size_type ii = 0; ii < components.size(); ++ii) {
    writelist[components[ii]].push_back(ii);
  }

  sort(writelist.begin(), writelist.end(), set_size_sort);

  return currentComponent;
}

/////////////////////////////////////////////////////////////////////////

void addAllEdgesFromVertices(const Graph<FloatType>& g,
                             std::vector<int>& components,
                             ProcessList& process) {
  Graph<FloatType>::vertex_descriptor vother;
  Graph<FloatType>::out_edge_iterator oe, oend;
  if (process.empty()) {
    return;
  }
  ProcessList process2;
  for (ProcessList::iterator ii = process.begin(); ii != process.end(); ++ii) {
    for (std::tie(oe, oend) = out_edges(*ii, g.boostGraph()); oe != oend;
         ++oe) {
      vother = target(*oe, g.boostGraph());
      if (components[vother] >= 0) {
        continue;
      }
      components[vother] = components[*ii];
      process2.insert(vother);
    }
  }
  process = process2;
}

void displayUsage(char** argv) {
  std::cerr << "\nUsage: " << argv[0]
            << " [-d outputDirectory] [-w doesWriteBool0or1]\n\t"
            << "[-c cutoff] [-s] graph.lgl\n\n"
            << "\t-s\tToggle new .lgl file write.\n"
            << "\n\tDefault output dir: " << doutputdir << '\n'
            << "\tDefault Write: " << defaultWrite << '\n'
            << "\tDefault Cutoff: " << cutoff << '\n'
            << "\tDoes Write New Lgl: " << defaultDoesWriteLgl << std::endl;
  exit(EXIT_FAILURE);
}
