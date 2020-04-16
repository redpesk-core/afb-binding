# Header files for developing bindings of Application Framework Binder

This project provides the header files and their documentation for
building microservice architecture components called bindings of the
application framework binder.

This project is available there https://github.com/redpesk/afb-binding.

## License and copying

See file LICENSE.txt for detail

## Building

The project has no dependencies.

Using cmake:

        mkdir build
        cd build
        cmake -DCMAKE_INSTALL_PREFIX=/usr/local ..
        make -j install

or use mkbuild:

        ./mkbuild.sh -p /usr/local install

## Usage with pkg-config

It installs a packege config file of name afb-binding.

That configuration file defines the cflags and libs to use
as usual.

Building a binding is possible for many targets. If the
binding is intended to be a shared object compatible with
afb-daemon, you should compile with option -fPIC.

In that case, when linking, you can use the provided version script
to reduce the set of exported symbols. To get the path of the
version script, use the command

        pkg-config --variable=version_script afb-binding
