# Background

For the EMMC version, the following examples are introduced.

The following demos can run normally in Ubuntu 16.04.


# Directory structure 

Capture_video_emmc:display  

Load_a_model_emmc:single model, face recognition   

Load_two_model_emmc:two model, License plate recognition   

Update_App:udpate firmware  

Update_one_blob:update one blob  

Update_two_blob:update two blob  

# Usage

The usage of each Demo is in README.md under the corresponding directory. 

**NVIDIA (arm64)**

Because opencv installed is 4.1.1, so you need to change the content in your Makefile.

Change 
```shell
OPENCV = `pkg-config opencv --cflags --libs`
```
to 
```shell
OPENCV = `pkg-config opencv4 --cflags --libs`
```

add `$(OPENCV)` after `COMMFLAG = -DUSE_WD`
like `COMMFLAG = -DUSE_WD $(OPENCV)`

then

```makefile
$ make run
```

# Maintainers

Beam.

Zed.



