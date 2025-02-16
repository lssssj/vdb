cmake .. \
  -DBUILD_TESTING=off \
  -DFAISS_ENABLE_PERFTEST=OFF \
  -DFAISS_ENABLE_GPU=OFF \
  -DFAISS_ENABLE_PYTHON=OFF \
  -DOpenMP_ROOT="$(brew --prefix libomp)"

cmake .. \
  -DOpenMP_ROOT="$(brew --prefix libomp)"

mkdir build && cd build && cmake .. -DOpenMP_ROOT="$(brew --prefix libomp)"  