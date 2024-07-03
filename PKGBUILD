pkgname=dnsch
pkgver=2.0
pkgdesc="A simple package that helps you to set your linux system DNS servers to several DNS servers"
url="https://github.com/mohammadrostamiorg/dnsChanger"
arch=('any')
license=('MIT')
maintainer="Mohammad Rosstami <mohammad.jayant@gmail.com>"
source=("$pkgname-$pkgver.tar.gz")
depends=('gcc' 'make' 'pthread')
pkgrel=1
sha256sums=('SKIP')
build() {
  cd "$pkgname-$pkgver"
  make
}
package() {
  cd "$pkgname-$pkgver"
  install -Dm755 dnsch "$pkgdir/usr/bin/dnsch"
}
