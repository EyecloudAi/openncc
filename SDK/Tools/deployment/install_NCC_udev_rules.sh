#Install OpenNCC USB driver

echo "Updating udev rules for OpenNCC..."


if [ -f "$(pwd)/97-myriad-ncc.rules" ]; then
    sudo usermod -a -G users "$(whoami)"

    sudo cp "$(pwd)/97-myriad-ncc.rules" /etc/udev/rules.d/
    sudo udevadm control --reload-rules
    sudo udevadm trigger
    sudo ldconfig
    echo "Udev rules have been successfully installed."
else
    echo "File '97-myriad-ncc.rules' is missing. Please make sure you installed 'Inference Engine Runtime for Intel® Movidius™ VPU'."
    exit -1
fi 


