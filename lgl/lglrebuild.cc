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
///////////////////////////////////////////////////////////

#include <unistd.h>

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "lgl/lib/calc_funcs.h"
#include "lgl/lib/configs.h"
#include "lgl/lib/cube.h"
#include "lgl/lib/ed_lookup_table.h"
#include "lgl/lib/grid.h"
#include "lgl/lib/molecule.h"
#include "lgl/lib/particle.h"
#include "lgl/lib/sphere.h"
#include "lgl/lib/voxel.h"
#include "lgl/lib/voxel_interaction_handler.h"

using namespace lgl::lib;

///////////////////////////////////////////////////////////

typedef float prec_t;
typedef Sphere particle;
typedef Molecule<lgl::lib::Dimension::k2Dimensions> Mol;
typedef Mol::vec_type vec_type;
typedef std::vector<Mol> Molecules;
typedef Mol::size_type size_type;
typedef EDLookupTable<particle> EDTable;
typedef std::vector<prec_t> EllipseFactors;

///////////////////////////////////////////////////////////

prec_t defaultradius = 1.0;
const char* defaultoutfile = "results.coords";
prec_t defaultstepsize = .49;

///////////////////////////////////////////////////////////

void displayUsage(char** args);
void writeResults(const Mol& m, const EllipseFactors& ellipseFactors,
                  const char* outfile);
void addMoleculeToTable(Mol& m, EDTable& table);
bool checkMoleculeAgainstTable(const Mol& m, EDTable& table);
void loadFilesFromList(const char* file, Molecules& m, prec_t radius);

///////////////////////////////////////////////////////////

const int GRAVITATIONAL = 0;
const int DLA = 1;

///////////////////////////////////////////////////////////

int main(int argc, char** argv) {
  // There is just one input arg,
  // a file name with all the connections.
  if (argc == 1) {
    displayUsage(argv);
  }

  const char* outfile = defaultoutfile;
  char* fileList = 0;
  prec_t radius = defaultradius;
  prec_t stepSize = defaultstepsize;
  int integrateType = GRAVITATIONAL;
  bool sortSetsFirst = true;
  EllipseFactors ellipseFactors(1, 1);

  int optch;
  while ((optch = getopt(argc, argv, "r:o:s:dc:Se:")) != -1) {
    switch (optch) {
      case 'r':
        radius = (prec_t)atof(optarg);
        break;
      case 'o':
        outfile = strdup(optarg);
        break;
      case 's':
        stepSize = (prec_t)atof(optarg);
        break;
      case 'd':
        integrateType = DLA;
        break;
      case 'c':
        fileList = strdup(optarg);
        break;
      case 'S':
        sortSetsFirst = false;
        break;
      case 'e':
        ellipseFactors = parseEllipseFactors(optarg);
        break;
      default:
        std::cerr << "Bad Option. Exiting.";
        exit(EXIT_FAILURE);
    }
  }

  // Load all the input files
  Molecules molecules;
  if (fileList == 0) {
    for (int fileCtr = optind; fileCtr < argc; ++fileCtr) {
      molecules.push_back(
          readMoleculeFromCoordFile<Mol>(argv[fileCtr], radius));
    }
  } else {
    loadFilesFromList(fileList, molecules, radius);
  }

  // In the event that only one molecule was provided, then
  // the simulation is over.
  if (argc == 2) {
    writeResults(molecules[0], ellipseFactors, outfile);
    exit(EXIT_SUCCESS);
  }

  // Sort molecules from largest to smallest. This will eventually
  // put the larger molecules at the center of the layout.
  if (sortSetsFirst)
    sort(molecules.begin(), molecules.end(),
         molecular_size_based_test<n_dimensions>);
  else
    random_shuffle(molecules.begin(), molecules.end());
  // printMolecules( cout ,  molecules.begin() , molecules.end() );

  // growth represents the DLA
  Mol growth(molecules.back());
  molecules.pop_back();  // Don't need this copy
  EDTable table(growth.dimension());
  addMoleculeToTable(growth, table);
  vec_type empty_vec(molecules[0].dimension(), 0);
  particle step_sphere(std::string("Step Sphere"), empty_vec, stepSize);

  int ctr = 1;
  std::cerr << "Total Total Connected Sets : " << std::setw(8)
            << molecules.size() << '\n';
  std::cerr << "Current Connected Set      : " << std::setw(8) << ctr
            << std::flush;

  Molecules::reverse_iterator diff_mol = molecules.rbegin();
  while (diff_mol != molecules.rend()) {
    // Set the largest molecule at origin.
    vec_type origin = simpleAverageMoleculePosition(growth);

    // Set seed sphere ( DLA starting point )
    particle seed(std::string("Seed Sphere"), origin);

    // Set kill sphere ( outer limit for diffusion )
    particle limit(std::string("Kill Sphere"), origin);

    // Set the radius of the set sphere. Must
    // check to see it is big enough for the growth
    // and the diffusing molecule
    prec_t radiusOfGrowth = radiusOfMolecule(growth);
    prec_t radiusOfDiffusingMolecule = radiusOfMolecule(*diff_mol);

    seed.radius(radiusOfGrowth + radiusOfDiffusingMolecule + 5 * stepSize);
    limit.radius(radiusOfGrowth + 2 * radiusOfDiffusingMolecule + 5 * stepSize);

    // Initialize the diffusing particles on the see sphere.
  INITIALIZE_MOLECULE:
    Mol::vec_type point;
    randomPointOnSurface(seed, point);
    moveMolecule(*diff_mol, point);

    // Start diffusing
    bool stillDiffusing = true;
    while (stillDiffusing) {
      // Determine next step for molecule
      vec_type next_step(0);
      if (integrateType == DLA)
        randomPointOnSurface(step_sphere, next_step);
      else {
        next_step = origin;
        vec_type mpoint(point);
        scale(mpoint.begin(), mpoint.end(), -1);
        translate(next_step.begin(), next_step.end(), mpoint.begin());
        prec_t m = magnitude(next_step.begin(), next_step.end());
        scale(next_step.begin(), next_step.end(), stepSize / m);
      }
      diff_mol->translateMolecule(next_step);

      // Has the molecule left the kill ring
      if (!isMoleculeRoughlyInSphere(*diff_mol, limit)) {
        goto INITIALIZE_MOLECULE;
      } else {
        // cout << "STILL IN" << endl;
      }

      // Has the molecule hit the growth...
      if (growth.inRange(*diff_mol) &&
          checkMoleculeAgainstTable(*diff_mol, table)) {
        stillDiffusing = false;
        std::cerr << "\b\b\b\b\b\b\b\b" << std::setw(8) << ctr++ << std::flush;
      } else {
        // cout << "STILL GOING" << endl;
      }
    }

    growth.addMolecule(*diff_mol);
    // Molecule is added to the growth, so ditch it
    addMoleculeToTable(*diff_mol, table);
    // diff_mol->clear();

    ++diff_mol;
  }

  // Shift all the growth to the 0
  // lower limit
  vec_type min(growth.min());
  scale(min.begin(), min.end(), -1.0);
  growth.translateMolecule(min);

  writeResults(growth, ellipseFactors, outfile);

  return EXIT_SUCCESS;
}

///////////////////////////////////////////////////////////

void displayUsage(char** args) {
  std::cerr << "\nUsage: " << args[0] << " [-r radius] [-o outfile] "
            << "[-s stepsize] [-c filelist]\n\t[-e ellipsefactors] [-S] [-d] "
               "coordsfile1 coordsfile2 ...\n\n"
            << "\tDefault outfile  : " << defaultoutfile << '\n'
            << "\tDefault radius   : " << defaultradius << '\n'
            << "\tDefault StepSize : " << defaultstepsize << '\n';
  exit(EXIT_FAILURE);
}

///////////////////////////////////////////////////////////

void writeResults(const Mol& m, const EllipseFactors& ellipseFactors,
                  const char* outfile) {
  std::ofstream out(outfile);
  if (!out) {
    std::cerr << "writeResults: Open of " << outfile << " failed.\n";
    exit(EXIT_FAILURE);
  }
  for (size_type jj = 0; jj < m.size(); ++jj) {
    particle p = m[jj];
    particle::vec_type& loc = p.location();
    for (size_type i = 0; i < loc.size(); ++i)
      loc[i] *=
          i < ellipseFactors.size() ? ellipseFactors[i] : ellipseFactors.back();

    p.printBasic(out);
  }
}

///////////////////////////////////////////////////////////

bool checkMoleculeAgainstTable(const Mol& m, EDTable& table) {
  typedef Mol::size_type size_type;
  // typedef EDTable::ResultList EL;

  for (size_type ii = 0; ii < m.size(); ++ii) {
    const particle& atom = m[ii];
    bool results = table.closeToEntries(atom.location(), atom.radius());
    if (results) {
      return true;
    }
  }

  return false;
}

///////////////////////////////////////////////////////////

void addMoleculeToTable(Mol& m, EDTable& table) {
  Mol::iterator b = m.atoms_begin(), e = m.atoms_end();
  for (; b != e; ++b) {
    table.insert(*b, b->location());
  }
}

///////////////////////////////////////////////////////////

void loadFilesFromList(const char* file, Molecules& m, prec_t radius) {
  std::ifstream in(file);
  if (!in) {
    std::cerr << "loadFilesFromList: Open of " << file << " failed.\n";
    exit(EXIT_FAILURE);
  }
  std::string coordsFile(128, ' ');
  while (in >> coordsFile && !in.eof()) {
    m.push_back(readMoleculeFromCoordFile<Mol>(coordsFile.c_str(), radius));
  }
}

///////////////////////////////////////////////////////////
