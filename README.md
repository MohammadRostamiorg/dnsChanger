# dnsChanger

A simple C program that helps you set your Linux DNS to several predefined or custom DNS servers.

## Installation

### Install with an AUR helper

#### Using yay

yay -S dnsch

#### Using paru

paru -S dnsch

### Install from source

1. Clone the project from GitHub:

git clone https://github.com/MohammadRostamiorg/dnsChanger.git 2. Navigate to the project directory:

cd dnsChanger

3. Build and install the package:

makepkg -sri

### or

sudo make install

## Usage

### Set predefined DNS servers

sudo dnsch {g|sh|ag|cf|403|bg|rd|el}

### Set custom DNS servers

sudo dnsch --set 1.2.3.4 5.6.7.8

### Clear DNS servers

sudo dnsch {-c|--clear}

### Get ping of several DNS servers

sudo dnsch {-p|--ping}

### Restore default DNS servers

sudo dnsch {-r|--restore}

## Note

This program requires root privileges to modify system DNS settings.
