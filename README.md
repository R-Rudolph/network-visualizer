network-visualizer
==================

A program which graphically displays network traffic in the local network.


### Description:

network-visualizer captures IPv4 packages with pcap and displays them graphically.

network-visualizer displays 3 kinds of sources/destinations of packets (displayed as big cubes):
* External: Addresses not from the local subnet are treated as external
* Local: Packets which are addressed as multicast or to addresses in the local subnet of which the program isn't sure if they exist are sent here
* Network Entities: Addresses in the local subnet of which the program knows that they exist. It knows, once a IP package from this address is captured.

Captured packages are displayed as smaller cubes (size dependent on packet size), starting at their respective source IP address, moving towards their destination. The color depends on the external IP address involved. Similar IP addresses result in similar colors.


### Controls:

* Left click on packet:  Stop packet (only visually, it will of course not stop actual network packages ;) )
* Right click on packet: Display information on packet (on console)
* W, A, S, D: Move camera
* Q, E, 8, 4, 6, 2: Rotate Camera
* I: Print camera information
* Ctrl+C: Exit
* Space: Pause (will stop packets from moving, also stops sniffing while paused)


### Libraries:

The libraries used are:
* libpcap
* libGL
* libglut (freeglut)
* libGLEW
* libSOIL
* glm
* pthread


### How to compile/dependencies:

In order to compile make and g++ are needed, as well as the libraries mentioned above.

Compile network-visualizer by executing

    make

This will generate the binary named network-visualizer.

### How to run

network-visualizer needs priviledged network access, it has to be executed either priviledged

    sudo ./network-visualizer

or given the proper capabilities before executing

    sudo setcap cap_net_raw+ep network-visualizer
