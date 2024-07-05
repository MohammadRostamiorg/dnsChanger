pkgname=dnsch
pkgver=2.0
pkgrel=1
pkgdesc="A simple package that helps you to set your Linux system DNS servers to several DNS servers"
url="https://github.com/mohammadrostamiorg/dnsChanger"
arch=('x86_64' 'i686' 'arm' 'armv6h' 'armv7h' 'aarch64')
license=('MIT')
maintainer="Mohammad Rosstami <mohammad.jayant@gmail.com>"
source=("$pkgname-$pkgver.tar.gz::$url/archive/v$pkgver.tar.gz")
depends=('glibc')
makedepends=('gcc' 'make')
sha256sums=('SKIP')

build() {
  cd "$srcdir/$pkgname-$pkgver"
  make
}

package() {
  cd "$srcdir/$pkgname-$pkgver"
  install -Dm755 dnsch "$pkgdir/usr/bin/dnsch"
  install -Dm644 dnsch.1 "$pkgdir/usr/share/man/man1/dnsch.1"
  install -Dm644 LICENSE "$pkgdir/usr/share/licenses/$pkgname/LICENSE"
}
