/////////////////////////////////////////////////////////////////////////

#include <iomanip>
#include <iostream>

#include "lgl/lib/graph.h"
#include "lgl/lib/io.h"

using namespace lgl::lib;

/////////////////////////////////////////////////////////////////////////

void usage(char** a);

/////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
  // There is just one input arg,
  // a file name with all the connections.
  if (argc != 3) usage(argv);

  std::string infile = argv[1];
  std::string outfile = argv[2];

  Graph<float> g;

  if ((infile.find(".ncol") != std::string::npos &&
       infile.find(".lgl") == std::string::npos) ||
      (outfile.find(".lgl") != std::string::npos &&
       outfile.find(".ncol") == std::string::npos)) {
    std::cerr << "Converting .ncol file ---> .lgl file\n";
    std::cerr << "Loading " << infile << "..." << std::flush;
    readNCOL(g, infile.c_str());
    std::cerr << " Done.\nWriting " << outfile << "..." << std::flush;
    writeLGL(g, outfile.c_str());
    std::cerr << " Done.\n";
  } else if ((infile.find(".lgl") != std::string::npos &&
              infile.find(".ncol") == std::string::npos) ||
             (outfile.find(".ncol") != std::string::npos &&
              outfile.find(".lgl") == std::string::npos)) {
    std::cerr << "Converting .lgl file ---> .ncol file\n";
    std::cerr << "Loading " << infile << "..." << std::flush;
    readLGL(g, infile.c_str());
    std::cerr << " Done.\nWriting " << outfile << "..." << std::flush;
    writeNCOL(g, outfile.c_str());
    std::cerr << " Done.\n";
  } else {
    std::cerr << "Could not determine file types.\n";
  }

  return EXIT_SUCCESS;
}

void usage(char** argv) {
  std::cerr << "\n Usage:\n\t" << argv[0]
            << " infile.ncol outfile.lgl\n\n\tOR\n\n\t";
  std::cerr << argv[0] << " infile.lgl outfile.ncol\n\n";
  exit(EXIT_FAILURE);
}
