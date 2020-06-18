# Achain2.0
the next generation of blockchain  technique Achain 


### Binary Releases[Recommended]
Achain2.0 currently supports Linux x86_64 RPM / Debian packages.
#### Centos RPM Package Install
```sh
$ wget https://github.com/Achain-Dev/Achain2.0/releases/download/v1.1.0/achain-1.1.0-1.x86_64.rpm
$ sudo yum install ./achain-1.1.0-1.x86_64.rpm
```

#### Centos RPM Package Uninstall
```sh
$ sudo yum remove achain
```

#### Debian Package Install
```sh
$ wget https://github.com/Achain-Dev/Achain2.0/releases/download/v1.1.0/achain_1.1.0-1_amd64.deb
$ sudo apt install ./achain_1.1.0-1_amd64.deb
```

#### Debian Package Uninstall
```sh
$ sudo apt remove achain
```

#### Get Achain2.0 code
```sh
git clone `https://github.com/Achain-Dev/Achain2.0.git`  --recursive

# Compile Achain2.0
cd Achain2.0  
git checkout actx1.1
./act_build.sh

The above process takes about 20-30 minutes, depending on the network

# Install Achain2.0
sudo ./act_install.sh
```

# Write dapp

recommend [Contract Development Toolkit](https://github.com/Achain-Dev/act.cdt) **_`Version：1.0.0`_** to build your contract
