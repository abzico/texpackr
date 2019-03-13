# texpackr
Textures packer into texture atlas, spit out atlas, and meta data. Aim to support .png, and .dds

# Build

* `make` - to build all included library (as object files), cli (command line program), and test program
* `make test` - to build library (as object files), and test program
* `make clean` - clean generated files and directory (`build` directory)

To **not** have texpackr spits out debugging log along the way, specify `-DTEXPACKR_NDEUB`

* `make CFLAGS=-DTEXPACKR_NDEBUG` or `make test CFLAGS=-DTEXPACKR_NDEBUG`

# TODO

## Phase I
- [x] packing algorithm for multiple images into a single texture atlas
- [x] read .png (support 1-4 channels: gray, gray-alpha, RGB, and RGBA) image as input
- [ ] save into a texture atlas as .png image
- [ ] read .jpg image as input
- [ ] save into a texture atlas as .dds image
- [ ] save meta data into file (file extension will be decided later)
- [x] C API

## Phase II
- [ ] ability to scale input images then output it
- [ ] cut out alpha pixels to maximize capacity of sprite storage in result spritesheet
- [ ] spacing between sprites packed in sheet

# License
MIT, ABZI
