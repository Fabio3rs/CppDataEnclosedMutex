# Data Enclosed Mutex - C++ Scoped Locking Utility

Data Enclosed Mutex is a C++ utility that provides scoped locking for objects protected by mutexes. It offers a convenient way to manage locks and ensure proper unlocking upon scope exit, helping to prevent deadlocks and resource leaks in multi-threaded applications.

## Disclaimer
Data Enclosed Mutex is a concept study project and may not be suitable for production use. Use it at your own risk.

## Features

- Scoped locking for objects protected by mutexes
- Support for both exclusive and shared locks
- Easy-to-use API with minimal overhead
- Thread-safe design
- Customizable lock types and guard behavior

## Usage

To use Data Enclosed Mutex in your project, simply include the necessary header file(s) and instantiate `Mutex` objects with appropriate initial values. You can then use the `lock` method to acquire a lock and safely access and modify the guarded data within the scope.

Example usage:

```cpp
#include "data_enclosed_mutex.hpp"

int main() {
    Mutex<int> dataMutex(0);

    {
        auto guard = dataMutex.lock();

        (*guard) = 42;
    }

    return 0;
}
```

## Installation
Data Enclosed Mutex is a header-only library, so there's no need for installation. Simply copy the header file `data_enclosed_mutex.hpp` into your project directory and include them in your source files.

See some examples in the `test` file, the `main.cpp` file, and the `data_enclosed_mutex.hpp` file for more information on how to use Data Enclosed Mutex.
See compilation steps in cmake-single-platform.yml

## Contributing
Contributions to Data Enclosed Mutex are welcome! If you find any bugs, have feature requests, or want to contribute enhancements or fixes, please open an issue or submit a pull request on the GitHub repository.

## License
Data Enclosed Mutex is licensed under the MIT License. See the LICENSE file for details.
