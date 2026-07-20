# Voxel Engine
---

Prior to reading this file, make sure you have taken notice of its [license](https://github.com/Allosker/voxel_engine/blob/main/license.txt " read carefully ").

--- 

## Development Checklist

1. Voxel \
Voxels shall be defined by only three attributes:
	* An ID represented by an unsigned 16-bits integer
	* A light value represented by a unsigned 16-bit integer: RGB S (where S denotes sun light), and where each component takes exactly 4 bits, number range [0;15] 

2. Voxel ID Manager \
This manager, represented as a singleton class; this class, accessible from anywhere, must only provide the data that exist for a given voxel type. Through a set of functions.