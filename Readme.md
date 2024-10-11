# C++23 ASCII Ray Tracer

This project is a simple ray tracer implemented in C++23 that renders 3D scenes as ASCII art in the console. It demonstrates the use of modern C++ features, parallel processing with OpenMP, and basic 3D graphics concepts.

## Features

- Renders spheres with basic shading
- Parallel rendering using OpenMP
- Real-time rendering progress display
- ASCII art output in the console
- Utilizes C++23 features

## Requirements

- C++23 compatible compiler (e.g., GCC 11 or later)
- CMake 3.12 or later
- OpenMP
- [rang library](https://github.com/agauniyal/rang) (automatically fetched by CMake)

## Building the Project

1. Clone the repository:
   ```
   git clone https://github.com/cschladetsch/cpp23-ascii-ray-tracer.git
   cd cpp23-ascii-ray-tracer
   ```

2. Create a build directory and navigate to it:
   ```
   mkdir build
   cd build
   ```

3. Run CMake and build the project:
   ```
   cmake ..
   make
   ```

## Running the Ray Tracer

After building, you can run the ray tracer with:

```
./ray_tracer
```

The program will display the rendering progress and then show the final ASCII art render in the console.

## Customizing the Scene

You can modify the `main.cpp` file to change the scene composition, add or remove spheres, or adjust their positions and colors.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## License

This project is open source and available under the [MIT License](LICENSE).

## Acknowledgements

- [rang library](https://github.com/agauniyal/rang) for terminal styling
- OpenMP for parallel processing support
```

