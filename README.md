# texpackr
Textures packer into texture atlas, spit out atlas, and meta data. Aim to support .png, and .dds

# Build

* `make` - to build all included library (as object files), cli (command line program), and test program
* `make test` - to build library (as object files), and test program
* `make clean` - clean generated files and directory (`build` directory)

To have texpackr spits out debugging log along the way, build it with

* `make CFLAGS=-DTEXPACKR_DEBUG` or `make test CFLAGS=-DTEXPACKR_DEBUG`

# TODO

- [x] packing algorithm for multiple images into a single texture atlas
- [x] read .png image as input
- [ ] read .jpg image as input
- [ ] save into a texture atlas as .tga file (purposely for debugging as .tga is easy to work with)
- [ ] save into a texture atlas as .png image
- [ ] save into a texture atlas as .dds image
- [ ] save meta data into file (file extension will be decided later)
- [x] C API
- [ ] ability to scale input images then output it
- [ ] cut out alpha pixels to maximize capacity of sprite storage in result spritesheet

# License
MIT, ABZI
