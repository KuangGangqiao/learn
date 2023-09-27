if [ ! -n "$1" ] ;then
    echo "You have not input python version!"
    exit -1
fi

whereis python
sudo rm /usr/bin/python
sudo ln -s /usr/bin/python$1 /usr/bin/python
