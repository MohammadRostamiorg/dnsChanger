# dnsChanger

#### a simple package that helps you to set your  linux dns to several DNS servers


## how to install
### Install with an aur helper
#### install with yay
```
yay -S dnsch
```
#### install with paru
```
paru -S dnsch
```

### install from source

##### Clone Project from github :
```
git clone https://github.com/MohammadRostamiorg/dnsChanger.git
```
##### Go to project directory
```
cd dnsChanger
```
##### install package
```
makepkg -sri
```


## how to use

#### Set serval DNS servers 
```
sudo dnsch {g|sh|ag|cf|403|bg|rd|el}
```

#### Set your custom DNS servers 
```
sudo dnsch --set 1.2.3.4 1.2.3.4
```

#### Clear DNS servers 
```
sudo dnsch {-c|--clear}
```

#### Get ping of serveal DNS servers 
```
sudo dnsch {-p|--ping}
```
#### Resotre default DNS serves
```
sudo dnsch {-r|--restore}
