#include <chrono>
#include <fstream>
#include <iostream>
#include <valarray>
#include <vector>
#include "predictor.hpp"

int main(int argc, char* argv[])
{
  if (argc != 2) {
    std::cout << "Error argument" << std::endl;
    return 1;
  }

  std::ifstream fin(argv[1]);

  if (!fin) {
    std::cout << "Input file does not exist" << std::endl;
    return 0;
  }

  pred::Predictor predictor;

  predictor.initialize(WIN_RDY_FILE_PATH);

  int m;
  std::vector<int> wall(K, 4);
  std::vector<int> river(K, 0);

  fin >> m;

  for (int i = 0; i < K; ++i) {
    fin.ignore(3, ' ');
    fin >> wall[i] >> river[i];
  }

  auto start = std::chrono::system_clock::now();

  std::vector<pred::Value> values;
  std::valarray<double> props;

  try {
    values = predictor(wall, river, m);
    props = values[0].props();
  }
  catch (std::runtime_error& e) {
    std::cout << e.what() << std::endl;
    return 0;
  }

  auto end = std::chrono::system_clock::now();

  std::cout << "Total\t" << values[0].all << std::endl;
  std::cout << "Wait\tNormalHand\tSevenPairs\tThirteenOrphans\tProportion" << std::endl;

  for (int i = 0; i < K; ++i) {
    std::cout << i << "\t"
              << values[1].states[i] << "\t"
              << values[2].states[i] << "\t"
              << values[3].states[i] << "\t"
              << props[i] << std::endl;
  }

  std::cout << "Time (msec.)\t"
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
            << std::endl;

  return 0;
}
