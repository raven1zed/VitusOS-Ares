unset shellHook
PATH=${PATH:-}
nix_saved_PATH="$PATH"
XDG_DATA_DIRS=${XDG_DATA_DIRS:-}
nix_saved_XDG_DATA_DIRS="$XDG_DATA_DIRS"
AR='ar'
export AR
AS='as'
export AS
BASH='/nix/store/lw117lsr8d585xs63kx5k233impyrq7q-bash-5.3p3/bin/bash'
CC='gcc'
export CC
CMAKE_INCLUDE_PATH='/nix/store/47y76bq8qcnpxizqgl0c0j15s7ciix5r-imagemagick-7.1.2-11-dev/include:/nix/store/wflnb8fxmq4lqx3ca91hlpv95vrqkv51-curl-8.17.0-dev/include:/nix/store/aq62vlf3liwp8rpgn6wp0wsf3wbhcpqb-brotli-1.2.0-dev/include:/nix/store/20dygnb42nwy7mbilwgql2lzz8h69pqz-krb5-1.22.1-dev/include:/nix/store/cxvk2hibqamfmgpbz9inbnvfyv49lr0h-nghttp2-1.67.1-dev/include:/nix/store/bcqpmsh9w6xdy7yd81nkaij8g3vmd61g-nghttp3-1.13.1-dev/include:/nix/store/ygwc32xg9dqylv1j3gh6rv2zlrhn4r7h-ngtcp2-1.18.0-dev/include:/nix/store/xy9v3ciqbqx2v9vqqn785hk22ndx55yd-libidn2-2.3.8-dev/include:/nix/store/2ivy0r8ab3bnps5957vfrxcjfcgad661-openssl-3.6.0-dev/include:/nix/store/p59c986zyf31f923yhr8sqxdqnq3plnh-libpsl-0.21.5-dev/include:/nix/store/dp3sg21kr75b2059s94g3a2vg8dfajmv-libssh2-1.11.1-dev/include:/nix/store/srnvjrr38j7xmpcmjpndd6mw31lsfjrl-zlib-1.3.1-dev/include:/nix/store/di94mxmhhk390zzzv2gvavfwkgww7q6h-zstd-1.5.7-dev/include:/nix/store/c828rss7q5rra33phplrb12ayv2p9vgw-bzip2-1.0.8-dev/include:/nix/store/ss8rcralsb3b192371ma7kn9wvl3kdx4-freetype-2.13.3-dev/include:/nix/store/li0sf0lhm30c83s1vi6filranamk9q3c-libpng-apng-1.6.52-dev/include:/nix/store/5ljn72xffsivv2ya5ncrzkyf5nx94227-libjpeg-turbo-3.1.3-dev/include:/nix/store/lb33y3zdr3dzc948mf9v8n6sznc5b9bf-lcms2-2.17-dev/include:/nix/store/vx5qrqwr1600p7db7ahykcgqbakk7gzc-libx11-1.8.12-dev/include:/nix/store/vfvnfnq9xb69gzxqv1s5kbpldi99bzr2-xorgproto-2024.1/include:/nix/store/s75hddpdhzzakb6jbzacq2q5bgh2y36k-libxt-1.3.1-dev/include:/nix/store/3xf66ysnxf1d5i37fnl144781iyva4q3-libsm-1.2.6-dev/include:/nix/store/5blg2pq37crjblq81sklbr7imya83wnb-libice-1.1.2-dev/include:/nix/store/i6742f2vdh0y6k1qyc0m7srk6h7ks0nh-libwebp-1.6.0/include:/nix/store/vlcfhfmsm11490mp7fd2n0g9rnapgpid-fftw-double-3.3.10-dev/include:/nix/store/n0cf9vad3va0js6606gcra8xhsnr0js7-gdb-16.3/include:/nix/store/n5m6p0j4anvf5cyy79nb2qn7smkzs176-gnumake-4.4.1/include:/nix/store/dk4dmm4bvskdjb84qp3yyg45mx3krysv-weston-14.0.1/include:/nix/store/w7rpcgzwhqdcrsxzj3ml4njjwv0bar82-compiler-rt-libc-21.1.7-dev/include:/nix/store/gwyq2ns8nzgf6q3cvr1k8gasfn03khcp-libcxx-21.1.7-dev/include:/nix/store/vxm9x8hynlq20jsm38waw7ji3qprd6rq-wlroots-0.19.2/include:/nix/store/c3q7cp0nbvndl38znhqw3daihnirqj3g-wayland-1.24.0-dev/include:/nix/store/zfl3y2dfih1qff84gldbkda1pz1lg45p-wayland-protocols-1.46/include:/nix/store/5s110bxdr4jvxi3wjd2dwwjv9inn8ddp-libxkbcommon-1.11.0-dev/include:/nix/store/sf0xrxsq0xr2iq96b8awrwwliik5dm6r-libdrm-2.4.129-dev/include:/nix/store/pa3asawv8vx96lz2zsm89rrg13flr4wk-libinput-1.29.2-dev/include:/nix/store/qhw6n40d6mi5ply6rf4gmf6b0vvg1wai-systemd-minimal-libs-258.2-dev/include:/nix/store/1cnvyk57f3wqlmbpidz0x2824ip9f8ly-pixman-0.46.4/include:/nix/store/8imqqzx7d62hfbn177lx18kqx08x8bma-seatd-0.9.1-dev/include:/nix/store/yydc0n1z5ag31x2vqv05n8iz498nvlm2-libglvnd-1.7.0-dev/include:/nix/store/ad2zpdj63hrnl90r01w9573m56gbkrh9-libffi-3.5.2-dev/include:/nix/store/nkgs0z67asdq920jxpkxazinzl11k0d3-libcap-2.77-dev/include:/nix/store/g3xkwyfnh4lifwyx6kmhnpz9bw2540hm-libxcb-1.17.0-dev/include:/nix/store/wd6lqkzpll686i5q9snd1c9c7glhcqqi-cairo-1.18.4-dev/include:/nix/store/pp2fbp19cabbdb2dhslf3qnxbsyyvnxm-fontconfig-2.17.1-dev/include:/nix/store/z7yvs4816g5xgmcj8122h0w97y5aa6l0-libxext-1.3.6-dev/include:/nix/store/fjp9qs2ww92pyag83fgq7amvqhph1dk7-libxau-1.0.12-dev/include:/nix/store/b4vxwmm7dr91x6ihh052k8ksxx5lybmj-libxrender-0.9.12-dev/include:/nix/store/j359ndglk11v5j47ra66lc1vxlk2dbjw-glib-2.86.3-dev/include:/nix/store/1dax567dy7cf55x25a1ryg6z6hl3lvzw-gettext-0.25.1/include:/nix/store/yxi1pmrgn8vsrrncz6pdlxhr5lsc3ysr-glibc-iconv-2.40/include:/nix/store/nd2qj4saqgbakhdnpl0f543lj9lalyfv-pango-1.57.0-dev/include:/nix/store/ayhcy9p6rshhda432gc1j3nqbhfklpi2-harfbuzz-12.2.0-dev/include:/nix/store/3salm1j8lhcyrkjblp7knj300raadc7g-graphite2-1.3.14-dev/include:/nix/store/21wibc25j73yapw3si0qbq1cyrv1dyfw-libxft-2.3.9-dev/include:/nix/store/bri9bpcyzlpcawwins7pinrsxrx69y6z-librsvg-2.61.3-dev/include:/nix/store/zsa9rgmanx1l9h7i76gfvv440n90vylg-gdk-pixbuf-2.44.4-dev/include:/nix/store/25y7kk3bbscnvvja6bixhz7wa95v2gvi-libtiff-4.7.1-dev/include:/nix/store/3mxwvjpsqvaairm8b7j495znp6l04j51-vulkan-headers-1.4.335.0/include:/nix/store/wgjgy1czmbdpq7i1p9kmcyhs5bn3p83c-glm-1.0.2/include'
export CMAKE_INCLUDE_PATH
CMAKE_LIBRARY_PATH='/nix/store/rj1cvkpc8nijyg7mv90c58s4kkhz3fiq-brotli-1.2.0-lib/lib:/nix/store/3n3j314vsy174dnwks8dd6c0vrqvh8h6-krb5-1.22.1-lib/lib:/nix/store/bhhlhxd7xzvmgyvxhjls4x09xfxfnrwc-nghttp2-1.67.1-lib/lib:/nix/store/qflgvhllyh98p52gh8wjlizfphhzsxb8-nghttp3-1.13.1/lib:/nix/store/m0xx7svz6967pw5knhqff8m0f48w0gj0-ngtcp2-1.18.0/lib:/nix/store/kywwgk85nl83mpf10av3bvm2khdlq5ib-libidn2-2.3.8/lib:/nix/store/rfm5m2l26lqkskcvxn5bm5xqh6c8wqr5-openssl-3.6.0/lib:/nix/store/yrfd45bhkyv1hbsvl81987ri326qbb3z-libpsl-0.21.5/lib:/nix/store/mxx1l7d0zhvqd9758rcbz0sbbaimkkq2-libssh2-1.11.1/lib:/nix/store/c2qsgf2832zi4n29gfkqgkjpvmbmxam6-zlib-1.3.1/lib:/nix/store/w07dqm9na06xgf9vbs0xqrlzya9p7g6b-zstd-1.5.7/lib:/nix/store/0apq92vpf3f3kll8vl9nhjq1gaaffibk-curl-8.17.0/lib:/nix/store/0c22zivvf0yspdvr2960rvmjgiwwi5wm-bzip2-1.0.8/lib:/nix/store/94dcyz2ssdhr0s2srnafpj49ia6shsn1-libpng-apng-1.6.52/lib:/nix/store/cvckjvajk2pdlx7i573fnd7ycrl46f17-freetype-2.13.3/lib:/nix/store/alddrrr7703gs9lhw7lknzwr51kj4jsl-libjpeg-turbo-3.1.3/lib:/nix/store/b8vsara78lvpyqz25n03qljh0587fqv5-lcms2-2.17/lib:/nix/store/ii3ybky5dqjikcrw7vdnh1j76ssy0ycm-libx11-1.8.12/lib:/nix/store/7yr1yymvbrkqyqxfp6hf0y7k2f8s91c7-libice-1.1.2/lib:/nix/store/s3syj0fs1a4rfi51fvgjmngagfyiiz9i-libsm-1.2.6/lib:/nix/store/5lm05s676bd9r6kgqnymbghzp6lg9xyw-libxt-1.3.1/lib:/nix/store/i6742f2vdh0y6k1qyc0m7srk6h7ks0nh-libwebp-1.6.0/lib:/nix/store/qpg5bb13x7p13d02kb8rv1f4333glzlw-fftw-double-3.3.10/lib:/nix/store/dfj4g2rpz8yf1g6ybj7d37p4kmwsl772-imagemagick-7.1.2-11/lib:/nix/store/n0cf9vad3va0js6606gcra8xhsnr0js7-gdb-16.3/lib:/nix/store/dk4dmm4bvskdjb84qp3yyg45mx3krysv-weston-14.0.1/lib:/nix/store/9d1kansnki5ipsfk9gv1ajb6fxd218ik-libcxx-21.1.7/lib:/nix/store/vxm9x8hynlq20jsm38waw7ji3qprd6rq-wlroots-0.19.2/lib:/nix/store/7m6x6b4ljrkmkbhvbxdcacpraxj49xkj-wayland-1.24.0/lib:/nix/store/91mg839gbvgz1svzlh1jvz0pqvx7ylwh-libxkbcommon-1.11.0/lib:/nix/store/sixic8a4adxz72swww4fcls0h5sabjdf-libdrm-2.4.129/lib:/nix/store/n234iswy40dzhd7aw6ak0wfrbyjpvbw4-systemd-minimal-libs-258.2/lib:/nix/store/pfaavvrfb1lhx26zwllc4vm5jyzfl1hh-libinput-1.29.2/lib:/nix/store/1cnvyk57f3wqlmbpidz0x2824ip9f8ly-pixman-0.46.4/lib:/nix/store/zjqacif3aa0idg3k18dlsz37iyzn0fqm-seatd-0.9.1/lib:/nix/store/x4sc437gdbhrfbm4b0hja9lfv38njvi1-libglvnd-1.7.0/lib:/nix/store/638y6yx9m2f5il7azqy1fn3hcczwjvfx-mesa-25.3.3/lib:/nix/store/1z031fwbsc8jhmm7i39r6z6m1xn7rbza-libffi-3.5.2/lib:/nix/store/n16jmmckg4fpqk97542hbdvlryxray79-libcap-2.77-lib/lib:/nix/store/m0jzvlr5gl7zvdn0fr7c09mjwx65fx58-libxcb-1.17.0/lib:/nix/store/va64srrrxljdrkh4w7y97gl3rzj6dgj9-fontconfig-2.17.1-lib/lib:/nix/store/whgxask7xm93jimlqrc2gm3j46p038gi-libxau-1.0.12/lib:/nix/store/zshby6nalhw4mvap0rr97hv042808c2k-libxext-1.3.6/lib:/nix/store/8xmc6wk01sgy6djqchvwf168vk6sp0qa-libxrender-0.9.12/lib:/nix/store/1dax567dy7cf55x25a1ryg6z6hl3lvzw-gettext-0.25.1/lib:/nix/store/f7rcazhd826xlcz43il4vafv28888cgj-glib-2.86.3/lib:/nix/store/733dh2bc55l8khcnkanmz1f99m77xjm0-cairo-1.18.4/lib:/nix/store/7n6bbv5ll4wpjq1kfbch7b16s74my77y-graphite2-1.3.14/lib:/nix/store/40b7x2n2yylv79rgnavw3xk7yp9308jx-harfbuzz-12.2.0/lib:/nix/store/wbnn8hmnfcd7pmk0n18fr7azsqrml1d0-libxft-2.3.9/lib:/nix/store/7qlq78jh6n35v1xpxyd6c279nbf0r88f-pango-1.57.0/lib:/nix/store/n1276dzpdi463s5nb2fsmysnxkirn3j6-libtiff-4.7.1/lib:/nix/store/5k9nfx38ckb0wkwsgy925ddzdhrvj16s-gdk-pixbuf-2.44.4/lib:/nix/store/8gz43a07r76vahgqlx88iz9yi22shdv0-librsvg-2.61.3/lib:/nix/store/jzng5i2xwsdlf3h60a5s6qzr2808c9zc-vulkan-loader-1.4.335.0/lib:/nix/store/94i9dxdbxpf6fyxsf7gviscnk5fs844x-vulkan-validation-layers-1.4.335.0/lib:/nix/store/wgjgy1czmbdpq7i1p9kmcyhs5bn3p83c-glm-1.0.2/lib'
export CMAKE_LIBRARY_PATH
CONFIG_SHELL='/nix/store/lw117lsr8d585xs63kx5k233impyrq7q-bash-5.3p3/bin/bash'
export CONFIG_SHELL
CXX='g++'
export CXX
GDK_PIXBUF_MODULE_FILE='/nix/store/8gz43a07r76vahgqlx88iz9yi22shdv0-librsvg-2.61.3/lib/gdk-pixbuf-2.0/2.10.0/loaders.cache'
export GDK_PIXBUF_MODULE_FILE
GETTEXTDATADIRS='/nix/store/1dax567dy7cf55x25a1ryg6z6hl3lvzw-gettext-0.25.1/share/gettext:/nix/store/f7rcazhd826xlcz43il4vafv28888cgj-glib-2.86.3/share/gettext'
export GETTEXTDATADIRS
HOSTTYPE='x86_64'
HOST_PATH='/nix/store/3mdkalklfdwfdwdi19x1zzlzl017y5xl-compiler-rt-libc-21.1.7/bin:/nix/store/91mg839gbvgz1svzlh1jvz0pqvx7ylwh-libxkbcommon-1.11.0/bin:/nix/store/0j7wz5lhxzzjnq637xyjv9arq6irplks-libdrm-2.4.129-bin/bin:/nix/store/248zkd6222jr7xv5jlwrdj70c15g3qvr-libinput-1.29.2-bin/bin:/nix/store/dyqhp6gciwzm31k2pc8g92rylqk4rfsz-seatd-0.9.1-bin/bin:/nix/store/638y6yx9m2f5il7azqy1fn3hcczwjvfx-mesa-25.3.3/bin:/nix/store/cm1jl79is5qb7zmqg8b8nmrdd7nqdjpk-libcap-2.77/bin:/nix/store/kg9a0vd3kq8p5hx40yw56z07v3vdlpif-xwayland-24.1.9/bin:/nix/store/wd6lqkzpll686i5q9snd1c9c7glhcqqi-cairo-1.18.4-dev/bin:/nix/store/ss8rcralsb3b192371ma7kn9wvl3kdx4-freetype-2.13.3-dev/bin:/nix/store/p61ba9fdgx3358bpp18hv4rslf6n5bq6-bzip2-1.0.8-bin/bin:/nix/store/sk8x7aabysn1wr89i6a813ffii4x7i8w-brotli-1.2.0/bin:/nix/store/li0sf0lhm30c83s1vi6filranamk9q3c-libpng-apng-1.6.52-dev/bin:/nix/store/5pzrv0bbcpw6pq4wi6vhjjv597ki2346-fontconfig-2.17.1-bin/bin:/nix/store/j359ndglk11v5j47ra66lc1vxlk2dbjw-glib-2.86.3-dev/bin:/nix/store/1dax567dy7cf55x25a1ryg6z6hl3lvzw-gettext-0.25.1/bin:/nix/store/4c9rdn3nsq3lv44zqsbb865fm174sxx9-glib-2.86.3-bin/bin:/nix/store/ayhcy9p6rshhda432gc1j3nqbhfklpi2-harfbuzz-12.2.0-dev/bin:/nix/store/7n6bbv5ll4wpjq1kfbch7b16s74my77y-graphite2-1.3.14/bin:/nix/store/qn4xlakafmwk72x5dj4bhlgssxim4xf0-pango-1.57.0-bin/bin:/nix/store/zsa9rgmanx1l9h7i76gfvv440n90vylg-gdk-pixbuf-2.44.4-dev/bin:/nix/store/7wg39dxpf0b4vigv4aib7hzqf6134la1-libtiff-4.7.1-bin/bin:/nix/store/35bk8ab4h86q4ymq0irz8ivy3m8q33hf-libjpeg-turbo-3.1.3-bin/bin:/nix/store/5k9nfx38ckb0wkwsgy925ddzdhrvj16s-gdk-pixbuf-2.44.4/bin:/nix/store/8gz43a07r76vahgqlx88iz9yi22shdv0-librsvg-2.61.3/bin:/nix/store/mc8i2kda3zpvfmz9mci7s1dsp3x5q448-vulkan-tools-1.4.335/bin:/nix/store/d75200gb22v7p0703h5jrkgg8bqydk5q-coreutils-9.8/bin:/nix/store/wd99g2j010fdkry0ws1bhxzm52w82ssx-findutils-4.10.0/bin:/nix/store/20zvyjvxq9x2mkp7rbnvrwjjzq2n76hh-diffutils-3.12/bin:/nix/store/k06ssckzrzn9jjvvs4n62m6567zmbx6x-gnused-4.9/bin:/nix/store/qfmq7p42ak5yn389qvx7zpxkan5i4xiy-gnugrep-3.12/bin:/nix/store/nmxm04dhkaqg1q6hai70n9zmzb0q49a5-gawk-5.3.2/bin:/nix/store/k1lcfin159706lihwx5hhvl80dbij4jw-gnutar-1.35/bin:/nix/store/0hv4z5s3r1h4lmvn0427mlxjxgvg34nr-gzip-1.14/bin:/nix/store/p61ba9fdgx3358bpp18hv4rslf6n5bq6-bzip2-1.0.8-bin/bin:/nix/store/mkm3my2067305hdh7rzmi10npwr7y17f-gnumake-4.4.1/bin:/nix/store/lw117lsr8d585xs63kx5k233impyrq7q-bash-5.3p3/bin:/nix/store/clrf4mjwr8xcfpvway6w34wzvqc1hry4-patch-2.8/bin:/nix/store/j1zc5jh0vi9sbxj09ldj4xklgm6kpf8n-xz-5.8.1-bin/bin:/nix/store/s2k48fw3y698j4kcvmw0520m06ihv2z4-file-5.45/bin'
export HOST_PATH
IFS=' 	
'
IN_NIX_SHELL='impure'
export IN_NIX_SHELL
LD='ld'
export LD
LINENO='76'
MACHTYPE='x86_64-pc-linux-gnu'
NIXPKGS_CMAKE_PREFIX_PATH='/nix/store/wmb16s8hw1vd25vvidwnncpzq126lcz3-cmake-4.1.2:/nix/store/4vxdgiak2ab3s853wz18wj11rmzw8bv6-ninja-1.13.2:/nix/store/hxn2qrz1zmk5q01wsb7n3d58brzrsizb-pkg-config-wrapper-0.29.2:/nix/store/6kc4z3z8x47dhxzbha5dkywp5qwji1y7-wayland-scanner-1.24.0-dev:/nix/store/ks51cz85rdrj4a374rfilgxf8b4c8d88-wayland-scanner-1.24.0-bin:/nix/store/rky12ys8gxzglv2wqykbi1y8cb0dc080-wayland-scanner-1.24.0:/nix/store/lcd1j6avhn4gaacgbf4wkxx2yz7y3mx2-Whitesur-icon-theme-2025-08-02:/nix/store/47y76bq8qcnpxizqgl0c0j15s7ciix5r-imagemagick-7.1.2-11-dev:/nix/store/wflnb8fxmq4lqx3ca91hlpv95vrqkv51-curl-8.17.0-dev:/nix/store/aq62vlf3liwp8rpgn6wp0wsf3wbhcpqb-brotli-1.2.0-dev:/nix/store/rj1cvkpc8nijyg7mv90c58s4kkhz3fiq-brotli-1.2.0-lib:/nix/store/sk8x7aabysn1wr89i6a813ffii4x7i8w-brotli-1.2.0:/nix/store/20dygnb42nwy7mbilwgql2lzz8h69pqz-krb5-1.22.1-dev:/nix/store/3n3j314vsy174dnwks8dd6c0vrqvh8h6-krb5-1.22.1-lib:/nix/store/26f9s2k3cx2iri2rjh229na2p5xfmhmb-krb5-1.22.1:/nix/store/cxvk2hibqamfmgpbz9inbnvfyv49lr0h-nghttp2-1.67.1-dev:/nix/store/bhhlhxd7xzvmgyvxhjls4x09xfxfnrwc-nghttp2-1.67.1-lib:/nix/store/r7l6n43ifx8c37qg4wckdms8w6pdlka8-nghttp2-1.67.1:/nix/store/bcqpmsh9w6xdy7yd81nkaij8g3vmd61g-nghttp3-1.13.1-dev:/nix/store/qflgvhllyh98p52gh8wjlizfphhzsxb8-nghttp3-1.13.1:/nix/store/ygwc32xg9dqylv1j3gh6rv2zlrhn4r7h-ngtcp2-1.18.0-dev:/nix/store/m0xx7svz6967pw5knhqff8m0f48w0gj0-ngtcp2-1.18.0:/nix/store/xy9v3ciqbqx2v9vqqn785hk22ndx55yd-libidn2-2.3.8-dev:/nix/store/wpq7nwgwlic1yiwnsm5b3brkig3nn50z-libidn2-2.3.8-bin:/nix/store/kywwgk85nl83mpf10av3bvm2khdlq5ib-libidn2-2.3.8:/nix/store/2ivy0r8ab3bnps5957vfrxcjfcgad661-openssl-3.6.0-dev:/nix/store/827b5xq1ghbbdrp1faa4fw32xj1m2p51-openssl-3.6.0-bin:/nix/store/rfm5m2l26lqkskcvxn5bm5xqh6c8wqr5-openssl-3.6.0:/nix/store/p59c986zyf31f923yhr8sqxdqnq3plnh-libpsl-0.21.5-dev:/nix/store/dlkas2sgry8a2zrcs8xs4j2g1qa1jfj5-publicsuffix-list-0-unstable-2025-11-14:/nix/store/yrfd45bhkyv1hbsvl81987ri326qbb3z-libpsl-0.21.5:/nix/store/dp3sg21kr75b2059s94g3a2vg8dfajmv-libssh2-1.11.1-dev:/nix/store/mxx1l7d0zhvqd9758rcbz0sbbaimkkq2-libssh2-1.11.1:/nix/store/srnvjrr38j7xmpcmjpndd6mw31lsfjrl-zlib-1.3.1-dev:/nix/store/c2qsgf2832zi4n29gfkqgkjpvmbmxam6-zlib-1.3.1:/nix/store/di94mxmhhk390zzzv2gvavfwkgww7q6h-zstd-1.5.7-dev:/nix/store/4i4q6abxjxnwf4j0ksc2zcb7byaxnrxi-zstd-1.5.7-bin:/nix/store/w07dqm9na06xgf9vbs0xqrlzya9p7g6b-zstd-1.5.7:/nix/store/cr9yz1bdfs9vj9cpr85ihrkgzgawgcwa-curl-8.17.0-bin:/nix/store/0apq92vpf3f3kll8vl9nhjq1gaaffibk-curl-8.17.0:/nix/store/c828rss7q5rra33phplrb12ayv2p9vgw-bzip2-1.0.8-dev:/nix/store/p61ba9fdgx3358bpp18hv4rslf6n5bq6-bzip2-1.0.8-bin:/nix/store/0c22zivvf0yspdvr2960rvmjgiwwi5wm-bzip2-1.0.8:/nix/store/ss8rcralsb3b192371ma7kn9wvl3kdx4-freetype-2.13.3-dev:/nix/store/li0sf0lhm30c83s1vi6filranamk9q3c-libpng-apng-1.6.52-dev:/nix/store/94dcyz2ssdhr0s2srnafpj49ia6shsn1-libpng-apng-1.6.52:/nix/store/cvckjvajk2pdlx7i573fnd7ycrl46f17-freetype-2.13.3:/nix/store/5ljn72xffsivv2ya5ncrzkyf5nx94227-libjpeg-turbo-3.1.3-dev:/nix/store/35bk8ab4h86q4ymq0irz8ivy3m8q33hf-libjpeg-turbo-3.1.3-bin:/nix/store/alddrrr7703gs9lhw7lknzwr51kj4jsl-libjpeg-turbo-3.1.3:/nix/store/lb33y3zdr3dzc948mf9v8n6sznc5b9bf-lcms2-2.17-dev:/nix/store/a6q5zzy75h8k8wa7mnzqcpvnnr1wj8jy-lcms2-2.17-bin:/nix/store/b8vsara78lvpyqz25n03qljh0587fqv5-lcms2-2.17:/nix/store/vx5qrqwr1600p7db7ahykcgqbakk7gzc-libx11-1.8.12-dev:/nix/store/vfvnfnq9xb69gzxqv1s5kbpldi99bzr2-xorgproto-2024.1:/nix/store/ii3ybky5dqjikcrw7vdnh1j76ssy0ycm-libx11-1.8.12:/nix/store/s75hddpdhzzakb6jbzacq2q5bgh2y36k-libxt-1.3.1-dev:/nix/store/3xf66ysnxf1d5i37fnl144781iyva4q3-libsm-1.2.6-dev:/nix/store/5blg2pq37crjblq81sklbr7imya83wnb-libice-1.1.2-dev:/nix/store/7yr1yymvbrkqyqxfp6hf0y7k2f8s91c7-libice-1.1.2:/nix/store/s3syj0fs1a4rfi51fvgjmngagfyiiz9i-libsm-1.2.6:/nix/store/5lm05s676bd9r6kgqnymbghzp6lg9xyw-libxt-1.3.1:/nix/store/i6742f2vdh0y6k1qyc0m7srk6h7ks0nh-libwebp-1.6.0:/nix/store/vlcfhfmsm11490mp7fd2n0g9rnapgpid-fftw-double-3.3.10-dev:/nix/store/qpg5bb13x7p13d02kb8rv1f4333glzlw-fftw-double-3.3.10:/nix/store/dfj4g2rpz8yf1g6ybj7d37p4kmwsl772-imagemagick-7.1.2-11:/nix/store/351bpjcf2l1n4vm06nwpq3cdhl6vbhx1-clang-wrapper-21.1.7:/nix/store/n21wcnpiydy3fd6kqxkwnzvwvaibmg11-binutils-wrapper-2.44:/nix/store/8ssdsraa07i1ciyl3az65xqnv44dfmx7-clang-wrapper-21.1.7:/nix/store/n0cf9vad3va0js6606gcra8xhsnr0js7-gdb-16.3:/nix/store/n5m6p0j4anvf5cyy79nb2qn7smkzs176-gnumake-4.4.1:/nix/store/fryiyyhhilpqhx2wc4b6l5gnxc8w18rg-git-2.52.0:/nix/store/dk4dmm4bvskdjb84qp3yyg45mx3krysv-weston-14.0.1:/nix/store/3vs2fr2mazafcdwyza15bfhpmccx1k7z-patchelf-0.15.2:/nix/store/9k7h0vvpnb8w4xcaypff4i76n1bsmwzz-update-autotools-gnu-config-scripts-hook:/nix/store/r9wbjib6xxjkyb9yvjvrkl4sq61i2lyn-gcc-wrapper-15.2.0:/nix/store/dwiyp91lmxq864plaas14jm14m87sg3f-binutils-wrapper-2.44:/nix/store/w7rpcgzwhqdcrsxzj3ml4njjwv0bar82-compiler-rt-libc-21.1.7-dev:/nix/store/3mdkalklfdwfdwdi19x1zzlzl017y5xl-compiler-rt-libc-21.1.7:/nix/store/gwyq2ns8nzgf6q3cvr1k8gasfn03khcp-libcxx-21.1.7-dev:/nix/store/9d1kansnki5ipsfk9gv1ajb6fxd218ik-libcxx-21.1.7:/nix/store/vxm9x8hynlq20jsm38waw7ji3qprd6rq-wlroots-0.19.2:/nix/store/c3q7cp0nbvndl38znhqw3daihnirqj3g-wayland-1.24.0-dev:/nix/store/7m6x6b4ljrkmkbhvbxdcacpraxj49xkj-wayland-1.24.0:/nix/store/zfl3y2dfih1qff84gldbkda1pz1lg45p-wayland-protocols-1.46:/nix/store/5s110bxdr4jvxi3wjd2dwwjv9inn8ddp-libxkbcommon-1.11.0-dev:/nix/store/91mg839gbvgz1svzlh1jvz0pqvx7ylwh-libxkbcommon-1.11.0:/nix/store/sf0xrxsq0xr2iq96b8awrwwliik5dm6r-libdrm-2.4.129-dev:/nix/store/0j7wz5lhxzzjnq637xyjv9arq6irplks-libdrm-2.4.129-bin:/nix/store/sixic8a4adxz72swww4fcls0h5sabjdf-libdrm-2.4.129:/nix/store/pa3asawv8vx96lz2zsm89rrg13flr4wk-libinput-1.29.2-dev:/nix/store/qhw6n40d6mi5ply6rf4gmf6b0vvg1wai-systemd-minimal-libs-258.2-dev:/nix/store/n234iswy40dzhd7aw6ak0wfrbyjpvbw4-systemd-minimal-libs-258.2:/nix/store/248zkd6222jr7xv5jlwrdj70c15g3qvr-libinput-1.29.2-bin:/nix/store/pfaavvrfb1lhx26zwllc4vm5jyzfl1hh-libinput-1.29.2:/nix/store/1cnvyk57f3wqlmbpidz0x2824ip9f8ly-pixman-0.46.4:/nix/store/8imqqzx7d62hfbn177lx18kqx08x8bma-seatd-0.9.1-dev:/nix/store/dyqhp6gciwzm31k2pc8g92rylqk4rfsz-seatd-0.9.1-bin:/nix/store/zjqacif3aa0idg3k18dlsz37iyzn0fqm-seatd-0.9.1:/nix/store/yydc0n1z5ag31x2vqv05n8iz498nvlm2-libglvnd-1.7.0-dev:/nix/store/x4sc437gdbhrfbm4b0hja9lfv38njvi1-libglvnd-1.7.0:/nix/store/638y6yx9m2f5il7azqy1fn3hcczwjvfx-mesa-25.3.3:/nix/store/ad2zpdj63hrnl90r01w9573m56gbkrh9-libffi-3.5.2-dev:/nix/store/1z031fwbsc8jhmm7i39r6z6m1xn7rbza-libffi-3.5.2:/nix/store/nkgs0z67asdq920jxpkxazinzl11k0d3-libcap-2.77-dev:/nix/store/n16jmmckg4fpqk97542hbdvlryxray79-libcap-2.77-lib:/nix/store/cm1jl79is5qb7zmqg8b8nmrdd7nqdjpk-libcap-2.77:/nix/store/g3xkwyfnh4lifwyx6kmhnpz9bw2540hm-libxcb-1.17.0-dev:/nix/store/m0jzvlr5gl7zvdn0fr7c09mjwx65fx58-libxcb-1.17.0:/nix/store/kg9a0vd3kq8p5hx40yw56z07v3vdlpif-xwayland-24.1.9:/nix/store/wd6lqkzpll686i5q9snd1c9c7glhcqqi-cairo-1.18.4-dev:/nix/store/pp2fbp19cabbdb2dhslf3qnxbsyyvnxm-fontconfig-2.17.1-dev:/nix/store/5pzrv0bbcpw6pq4wi6vhjjv597ki2346-fontconfig-2.17.1-bin:/nix/store/va64srrrxljdrkh4w7y97gl3rzj6dgj9-fontconfig-2.17.1-lib:/nix/store/z7yvs4816g5xgmcj8122h0w97y5aa6l0-libxext-1.3.6-dev:/nix/store/fjp9qs2ww92pyag83fgq7amvqhph1dk7-libxau-1.0.12-dev:/nix/store/whgxask7xm93jimlqrc2gm3j46p038gi-libxau-1.0.12:/nix/store/zshby6nalhw4mvap0rr97hv042808c2k-libxext-1.3.6:/nix/store/b4vxwmm7dr91x6ihh052k8ksxx5lybmj-libxrender-0.9.12-dev:/nix/store/8xmc6wk01sgy6djqchvwf168vk6sp0qa-libxrender-0.9.12:/nix/store/j359ndglk11v5j47ra66lc1vxlk2dbjw-glib-2.86.3-dev:/nix/store/1dax567dy7cf55x25a1ryg6z6hl3lvzw-gettext-0.25.1:/nix/store/yxi1pmrgn8vsrrncz6pdlxhr5lsc3ysr-glibc-iconv-2.40:/nix/store/4c9rdn3nsq3lv44zqsbb865fm174sxx9-glib-2.86.3-bin:/nix/store/f7rcazhd826xlcz43il4vafv28888cgj-glib-2.86.3:/nix/store/733dh2bc55l8khcnkanmz1f99m77xjm0-cairo-1.18.4:/nix/store/nd2qj4saqgbakhdnpl0f543lj9lalyfv-pango-1.57.0-dev:/nix/store/ayhcy9p6rshhda432gc1j3nqbhfklpi2-harfbuzz-12.2.0-dev:/nix/store/3salm1j8lhcyrkjblp7knj300raadc7g-graphite2-1.3.14-dev:/nix/store/7n6bbv5ll4wpjq1kfbch7b16s74my77y-graphite2-1.3.14:/nix/store/40b7x2n2yylv79rgnavw3xk7yp9308jx-harfbuzz-12.2.0:/nix/store/21wibc25j73yapw3si0qbq1cyrv1dyfw-libxft-2.3.9-dev:/nix/store/wbnn8hmnfcd7pmk0n18fr7azsqrml1d0-libxft-2.3.9:/nix/store/qn4xlakafmwk72x5dj4bhlgssxim4xf0-pango-1.57.0-bin:/nix/store/7qlq78jh6n35v1xpxyd6c279nbf0r88f-pango-1.57.0:/nix/store/bri9bpcyzlpcawwins7pinrsxrx69y6z-librsvg-2.61.3-dev:/nix/store/zsa9rgmanx1l9h7i76gfvv440n90vylg-gdk-pixbuf-2.44.4-dev:/nix/store/25y7kk3bbscnvvja6bixhz7wa95v2gvi-libtiff-4.7.1-dev:/nix/store/7wg39dxpf0b4vigv4aib7hzqf6134la1-libtiff-4.7.1-bin:/nix/store/n1276dzpdi463s5nb2fsmysnxkirn3j6-libtiff-4.7.1:/nix/store/5k9nfx38ckb0wkwsgy925ddzdhrvj16s-gdk-pixbuf-2.44.4:/nix/store/8gz43a07r76vahgqlx88iz9yi22shdv0-librsvg-2.61.3:/nix/store/3mxwvjpsqvaairm8b7j495znp6l04j51-vulkan-headers-1.4.335.0:/nix/store/23xwha5lvfrgdhfqw8vm62as3p5dych2-vulkan-loader-1.4.335.0-dev:/nix/store/jzng5i2xwsdlf3h60a5s6qzr2808c9zc-vulkan-loader-1.4.335.0:/nix/store/mc8i2kda3zpvfmz9mci7s1dsp3x5q448-vulkan-tools-1.4.335:/nix/store/94i9dxdbxpf6fyxsf7gviscnk5fs844x-vulkan-validation-layers-1.4.335.0:/nix/store/wgjgy1czmbdpq7i1p9kmcyhs5bn3p83c-glm-1.0.2'
export NIXPKGS_CMAKE_PREFIX_PATH
NIX_BINTOOLS='/nix/store/dwiyp91lmxq864plaas14jm14m87sg3f-binutils-wrapper-2.44'
export NIX_BINTOOLS
NIX_BINTOOLS_WRAPPER_TARGET_HOST_x86_64_unknown_linux_gnu='1'
export NIX_BINTOOLS_WRAPPER_TARGET_HOST_x86_64_unknown_linux_gnu
NIX_BUILD_CORES='12'
export NIX_BUILD_CORES
NIX_CC='/nix/store/r9wbjib6xxjkyb9yvjvrkl4sq61i2lyn-gcc-wrapper-15.2.0'
export NIX_CC
NIX_CC_WRAPPER_TARGET_HOST_x86_64_unknown_linux_gnu='1'
export NIX_CC_WRAPPER_TARGET_HOST_x86_64_unknown_linux_gnu
NIX_CFLAGS_COMPILE=' -frandom-seed=my21y4gcrz -isystem /nix/store/47y76bq8qcnpxizqgl0c0j15s7ciix5r-imagemagick-7.1.2-11-dev/include -isystem /nix/store/47y76bq8qcnpxizqgl0c0j15s7ciix5r-imagemagick-7.1.2-11-dev/include -isystem /nix/store/47y76bq8qcnpxizqgl0c0j15s7ciix5r-imagemagick-7.1.2-11-dev/include -isystem /nix/store/wflnb8fxmq4lqx3ca91hlpv95vrqkv51-curl-8.17.0-dev/include -isystem /nix/store/wflnb8fxmq4lqx3ca91hlpv95vrqkv51-curl-8.17.0-dev/include -isystem /nix/store/wflnb8fxmq4lqx3ca91hlpv95vrqkv51-curl-8.17.0-dev/include -isystem /nix/store/aq62vlf3liwp8rpgn6wp0wsf3wbhcpqb-brotli-1.2.0-dev/include -isystem /nix/store/aq62vlf3liwp8rpgn6wp0wsf3wbhcpqb-brotli-1.2.0-dev/include -isystem /nix/store/aq62vlf3liwp8rpgn6wp0wsf3wbhcpqb-brotli-1.2.0-dev/include -isystem /nix/store/20dygnb42nwy7mbilwgql2lzz8h69pqz-krb5-1.22.1-dev/include -isystem /nix/store/20dygnb42nwy7mbilwgql2lzz8h69pqz-krb5-1.22.1-dev/include -isystem /nix/store/20dygnb42nwy7mbilwgql2lzz8h69pqz-krb5-1.22.1-dev/include -isystem /nix/store/cxvk2hibqamfmgpbz9inbnvfyv49lr0h-nghttp2-1.67.1-dev/include -isystem /nix/store/cxvk2hibqamfmgpbz9inbnvfyv49lr0h-nghttp2-1.67.1-dev/include -isystem /nix/store/cxvk2hibqamfmgpbz9inbnvfyv49lr0h-nghttp2-1.67.1-dev/include -isystem /nix/store/bcqpmsh9w6xdy7yd81nkaij8g3vmd61g-nghttp3-1.13.1-dev/include -isystem /nix/store/bcqpmsh9w6xdy7yd81nkaij8g3vmd61g-nghttp3-1.13.1-dev/include -isystem /nix/store/bcqpmsh9w6xdy7yd81nkaij8g3vmd61g-nghttp3-1.13.1-dev/include -isystem /nix/store/ygwc32xg9dqylv1j3gh6rv2zlrhn4r7h-ngtcp2-1.18.0-dev/include -isystem /nix/store/ygwc32xg9dqylv1j3gh6rv2zlrhn4r7h-ngtcp2-1.18.0-dev/include -isystem /nix/store/ygwc32xg9dqylv1j3gh6rv2zlrhn4r7h-ngtcp2-1.18.0-dev/include -isystem /nix/store/xy9v3ciqbqx2v9vqqn785hk22ndx55yd-libidn2-2.3.8-dev/include -isystem /nix/store/xy9v3ciqbqx2v9vqqn785hk22ndx55yd-libidn2-2.3.8-dev/include -isystem /nix/store/xy9v3ciqbqx2v9vqqn785hk22ndx55yd-libidn2-2.3.8-dev/include -isystem /nix/store/2ivy0r8ab3bnps5957vfrxcjfcgad661-openssl-3.6.0-dev/include -isystem /nix/store/2ivy0r8ab3bnps5957vfrxcjfcgad661-openssl-3.6.0-dev/include -isystem /nix/store/2ivy0r8ab3bnps5957vfrxcjfcgad661-openssl-3.6.0-dev/include -isystem /nix/store/p59c986zyf31f923yhr8sqxdqnq3plnh-libpsl-0.21.5-dev/include -isystem /nix/store/p59c986zyf31f923yhr8sqxdqnq3plnh-libpsl-0.21.5-dev/include -isystem /nix/store/p59c986zyf31f923yhr8sqxdqnq3plnh-libpsl-0.21.5-dev/include -isystem /nix/store/dp3sg21kr75b2059s94g3a2vg8dfajmv-libssh2-1.11.1-dev/include -isystem /nix/store/dp3sg21kr75b2059s94g3a2vg8dfajmv-libssh2-1.11.1-dev/include -isystem /nix/store/dp3sg21kr75b2059s94g3a2vg8dfajmv-libssh2-1.11.1-dev/include -isystem /nix/store/srnvjrr38j7xmpcmjpndd6mw31lsfjrl-zlib-1.3.1-dev/include -isystem /nix/store/srnvjrr38j7xmpcmjpndd6mw31lsfjrl-zlib-1.3.1-dev/include -isystem /nix/store/srnvjrr38j7xmpcmjpndd6mw31lsfjrl-zlib-1.3.1-dev/include -isystem /nix/store/di94mxmhhk390zzzv2gvavfwkgww7q6h-zstd-1.5.7-dev/include -isystem /nix/store/di94mxmhhk390zzzv2gvavfwkgww7q6h-zstd-1.5.7-dev/include -isystem /nix/store/di94mxmhhk390zzzv2gvavfwkgww7q6h-zstd-1.5.7-dev/include -isystem /nix/store/c828rss7q5rra33phplrb12ayv2p9vgw-bzip2-1.0.8-dev/include -isystem /nix/store/c828rss7q5rra33phplrb12ayv2p9vgw-bzip2-1.0.8-dev/include -isystem /nix/store/c828rss7q5rra33phplrb12ayv2p9vgw-bzip2-1.0.8-dev/include -isystem /nix/store/ss8rcralsb3b192371ma7kn9wvl3kdx4-freetype-2.13.3-dev/include -isystem /nix/store/ss8rcralsb3b192371ma7kn9wvl3kdx4-freetype-2.13.3-dev/include -isystem /nix/store/ss8rcralsb3b192371ma7kn9wvl3kdx4-freetype-2.13.3-dev/include -isystem /nix/store/li0sf0lhm30c83s1vi6filranamk9q3c-libpng-apng-1.6.52-dev/include -isystem /nix/store/li0sf0lhm30c83s1vi6filranamk9q3c-libpng-apng-1.6.52-dev/include -isystem /nix/store/li0sf0lhm30c83s1vi6filranamk9q3c-libpng-apng-1.6.52-dev/include -isystem /nix/store/5ljn72xffsivv2ya5ncrzkyf5nx94227-libjpeg-turbo-3.1.3-dev/include -isystem /nix/store/5ljn72xffsivv2ya5ncrzkyf5nx94227-libjpeg-turbo-3.1.3-dev/include -isystem /nix/store/5ljn72xffsivv2ya5ncrzkyf5nx94227-libjpeg-turbo-3.1.3-dev/include -isystem /nix/store/lb33y3zdr3dzc948mf9v8n6sznc5b9bf-lcms2-2.17-dev/include -isystem /nix/store/lb33y3zdr3dzc948mf9v8n6sznc5b9bf-lcms2-2.17-dev/include -isystem /nix/store/lb33y3zdr3dzc948mf9v8n6sznc5b9bf-lcms2-2.17-dev/include -isystem /nix/store/vx5qrqwr1600p7db7ahykcgqbakk7gzc-libx11-1.8.12-dev/include -isystem /nix/store/vx5qrqwr1600p7db7ahykcgqbakk7gzc-libx11-1.8.12-dev/include -isystem /nix/store/vx5qrqwr1600p7db7ahykcgqbakk7gzc-libx11-1.8.12-dev/include -isystem /nix/store/vfvnfnq9xb69gzxqv1s5kbpldi99bzr2-xorgproto-2024.1/include -isystem /nix/store/vfvnfnq9xb69gzxqv1s5kbpldi99bzr2-xorgproto-2024.1/include -isystem /nix/store/vfvnfnq9xb69gzxqv1s5kbpldi99bzr2-xorgproto-2024.1/include -isystem /nix/store/s75hddpdhzzakb6jbzacq2q5bgh2y36k-libxt-1.3.1-dev/include -isystem /nix/store/s75hddpdhzzakb6jbzacq2q5bgh2y36k-libxt-1.3.1-dev/include -isystem /nix/store/s75hddpdhzzakb6jbzacq2q5bgh2y36k-libxt-1.3.1-dev/include -isystem /nix/store/3xf66ysnxf1d5i37fnl144781iyva4q3-libsm-1.2.6-dev/include -isystem /nix/store/3xf66ysnxf1d5i37fnl144781iyva4q3-libsm-1.2.6-dev/include -isystem /nix/store/3xf66ysnxf1d5i37fnl144781iyva4q3-libsm-1.2.6-dev/include -isystem /nix/store/5blg2pq37crjblq81sklbr7imya83wnb-libice-1.1.2-dev/include -isystem /nix/store/5blg2pq37crjblq81sklbr7imya83wnb-libice-1.1.2-dev/include -isystem /nix/store/5blg2pq37crjblq81sklbr7imya83wnb-libice-1.1.2-dev/include -isystem /nix/store/i6742f2vdh0y6k1qyc0m7srk6h7ks0nh-libwebp-1.6.0/include -isystem /nix/store/i6742f2vdh0y6k1qyc0m7srk6h7ks0nh-libwebp-1.6.0/include -isystem /nix/store/i6742f2vdh0y6k1qyc0m7srk6h7ks0nh-libwebp-1.6.0/include -isystem /nix/store/vlcfhfmsm11490mp7fd2n0g9rnapgpid-fftw-double-3.3.10-dev/include -isystem /nix/store/vlcfhfmsm11490mp7fd2n0g9rnapgpid-fftw-double-3.3.10-dev/include -isystem /nix/store/vlcfhfmsm11490mp7fd2n0g9rnapgpid-fftw-double-3.3.10-dev/include -isystem /nix/store/n0cf9vad3va0js6606gcra8xhsnr0js7-gdb-16.3/include -isystem /nix/store/n0cf9vad3va0js6606gcra8xhsnr0js7-gdb-16.3/include -isystem /nix/store/n0cf9vad3va0js6606gcra8xhsnr0js7-gdb-16.3/include -isystem /nix/store/n5m6p0j4anvf5cyy79nb2qn7smkzs176-gnumake-4.4.1/include -isystem /nix/store/n5m6p0j4anvf5cyy79nb2qn7smkzs176-gnumake-4.4.1/include -isystem /nix/store/n5m6p0j4anvf5cyy79nb2qn7smkzs176-gnumake-4.4.1/include -isystem /nix/store/dk4dmm4bvskdjb84qp3yyg45mx3krysv-weston-14.0.1/include -isystem /nix/store/dk4dmm4bvskdjb84qp3yyg45mx3krysv-weston-14.0.1/include -isystem /nix/store/dk4dmm4bvskdjb84qp3yyg45mx3krysv-weston-14.0.1/include -isystem /nix/store/w7rpcgzwhqdcrsxzj3ml4njjwv0bar82-compiler-rt-libc-21.1.7-dev/include -isystem /nix/store/w7rpcgzwhqdcrsxzj3ml4njjwv0bar82-compiler-rt-libc-21.1.7-dev/include -isystem /nix/store/w7rpcgzwhqdcrsxzj3ml4njjwv0bar82-compiler-rt-libc-21.1.7-dev/include -isystem /nix/store/gwyq2ns8nzgf6q3cvr1k8gasfn03khcp-libcxx-21.1.7-dev/include -isystem /nix/store/gwyq2ns8nzgf6q3cvr1k8gasfn03khcp-libcxx-21.1.7-dev/include -isystem /nix/store/gwyq2ns8nzgf6q3cvr1k8gasfn03khcp-libcxx-21.1.7-dev/include -isystem /nix/store/vxm9x8hynlq20jsm38waw7ji3qprd6rq-wlroots-0.19.2/include -isystem /nix/store/vxm9x8hynlq20jsm38waw7ji3qprd6rq-wlroots-0.19.2/include -isystem /nix/store/vxm9x8hynlq20jsm38waw7ji3qprd6rq-wlroots-0.19.2/include -isystem /nix/store/c3q7cp0nbvndl38znhqw3daihnirqj3g-wayland-1.24.0-dev/include -isystem /nix/store/c3q7cp0nbvndl38znhqw3daihnirqj3g-wayland-1.24.0-dev/include -isystem /nix/store/c3q7cp0nbvndl38znhqw3daihnirqj3g-wayland-1.24.0-dev/include -isystem /nix/store/zfl3y2dfih1qff84gldbkda1pz1lg45p-wayland-protocols-1.46/include -isystem /nix/store/zfl3y2dfih1qff84gldbkda1pz1lg45p-wayland-protocols-1.46/include -isystem /nix/store/zfl3y2dfih1qff84gldbkda1pz1lg45p-wayland-protocols-1.46/include -isystem /nix/store/5s110bxdr4jvxi3wjd2dwwjv9inn8ddp-libxkbcommon-1.11.0-dev/include -isystem /nix/store/5s110bxdr4jvxi3wjd2dwwjv9inn8ddp-libxkbcommon-1.11.0-dev/include -isystem /nix/store/5s110bxdr4jvxi3wjd2dwwjv9inn8ddp-libxkbcommon-1.11.0-dev/include -isystem /nix/store/sf0xrxsq0xr2iq96b8awrwwliik5dm6r-libdrm-2.4.129-dev/include -isystem /nix/store/sf0xrxsq0xr2iq96b8awrwwliik5dm6r-libdrm-2.4.129-dev/include -isystem /nix/store/sf0xrxsq0xr2iq96b8awrwwliik5dm6r-libdrm-2.4.129-dev/include -isystem /nix/store/pa3asawv8vx96lz2zsm89rrg13flr4wk-libinput-1.29.2-dev/include -isystem /nix/store/pa3asawv8vx96lz2zsm89rrg13flr4wk-libinput-1.29.2-dev/include -isystem /nix/store/pa3asawv8vx96lz2zsm89rrg13flr4wk-libinput-1.29.2-dev/include -isystem /nix/store/qhw6n40d6mi5ply6rf4gmf6b0vvg1wai-systemd-minimal-libs-258.2-dev/include -isystem /nix/store/qhw6n40d6mi5ply6rf4gmf6b0vvg1wai-systemd-minimal-libs-258.2-dev/include -isystem /nix/store/qhw6n40d6mi5ply6rf4gmf6b0vvg1wai-systemd-minimal-libs-258.2-dev/include -isystem /nix/store/1cnvyk57f3wqlmbpidz0x2824ip9f8ly-pixman-0.46.4/include -isystem /nix/store/1cnvyk57f3wqlmbpidz0x2824ip9f8ly-pixman-0.46.4/include -isystem /nix/store/1cnvyk57f3wqlmbpidz0x2824ip9f8ly-pixman-0.46.4/include -isystem /nix/store/8imqqzx7d62hfbn177lx18kqx08x8bma-seatd-0.9.1-dev/include -isystem /nix/store/8imqqzx7d62hfbn177lx18kqx08x8bma-seatd-0.9.1-dev/include -isystem /nix/store/8imqqzx7d62hfbn177lx18kqx08x8bma-seatd-0.9.1-dev/include -isystem /nix/store/yydc0n1z5ag31x2vqv05n8iz498nvlm2-libglvnd-1.7.0-dev/include -isystem /nix/store/yydc0n1z5ag31x2vqv05n8iz498nvlm2-libglvnd-1.7.0-dev/include -isystem /nix/store/yydc0n1z5ag31x2vqv05n8iz498nvlm2-libglvnd-1.7.0-dev/include -isystem /nix/store/ad2zpdj63hrnl90r01w9573m56gbkrh9-libffi-3.5.2-dev/include -isystem /nix/store/ad2zpdj63hrnl90r01w9573m56gbkrh9-libffi-3.5.2-dev/include -isystem /nix/store/ad2zpdj63hrnl90r01w9573m56gbkrh9-libffi-3.5.2-dev/include -isystem /nix/store/nkgs0z67asdq920jxpkxazinzl11k0d3-libcap-2.77-dev/include -isystem /nix/store/nkgs0z67asdq920jxpkxazinzl11k0d3-libcap-2.77-dev/include -isystem /nix/store/nkgs0z67asdq920jxpkxazinzl11k0d3-libcap-2.77-dev/include -isystem /nix/store/g3xkwyfnh4lifwyx6kmhnpz9bw2540hm-libxcb-1.17.0-dev/include -isystem /nix/store/g3xkwyfnh4lifwyx6kmhnpz9bw2540hm-libxcb-1.17.0-dev/include -isystem /nix/store/g3xkwyfnh4lifwyx6kmhnpz9bw2540hm-libxcb-1.17.0-dev/include -isystem /nix/store/wd6lqkzpll686i5q9snd1c9c7glhcqqi-cairo-1.18.4-dev/include -isystem /nix/store/wd6lqkzpll686i5q9snd1c9c7glhcqqi-cairo-1.18.4-dev/include -isystem /nix/store/wd6lqkzpll686i5q9snd1c9c7glhcqqi-cairo-1.18.4-dev/include -isystem /nix/store/pp2fbp19cabbdb2dhslf3qnxbsyyvnxm-fontconfig-2.17.1-dev/include -isystem /nix/store/pp2fbp19cabbdb2dhslf3qnxbsyyvnxm-fontconfig-2.17.1-dev/include -isystem /nix/store/pp2fbp19cabbdb2dhslf3qnxbsyyvnxm-fontconfig-2.17.1-dev/include -isystem /nix/store/z7yvs4816g5xgmcj8122h0w97y5aa6l0-libxext-1.3.6-dev/include -isystem /nix/store/z7yvs4816g5xgmcj8122h0w97y5aa6l0-libxext-1.3.6-dev/include -isystem /nix/store/z7yvs4816g5xgmcj8122h0w97y5aa6l0-libxext-1.3.6-dev/include -isystem /nix/store/fjp9qs2ww92pyag83fgq7amvqhph1dk7-libxau-1.0.12-dev/include -isystem /nix/store/fjp9qs2ww92pyag83fgq7amvqhph1dk7-libxau-1.0.12-dev/include -isystem /nix/store/fjp9qs2ww92pyag83fgq7amvqhph1dk7-libxau-1.0.12-dev/include -isystem /nix/store/b4vxwmm7dr91x6ihh052k8ksxx5lybmj-libxrender-0.9.12-dev/include -isystem /nix/store/b4vxwmm7dr91x6ihh052k8ksxx5lybmj-libxrender-0.9.12-dev/include -isystem /nix/store/b4vxwmm7dr91x6ihh052k8ksxx5lybmj-libxrender-0.9.12-dev/include -isystem /nix/store/j359ndglk11v5j47ra66lc1vxlk2dbjw-glib-2.86.3-dev/include -isystem /nix/store/j359ndglk11v5j47ra66lc1vxlk2dbjw-glib-2.86.3-dev/include -isystem /nix/store/j359ndglk11v5j47ra66lc1vxlk2dbjw-glib-2.86.3-dev/include -isystem /nix/store/1dax567dy7cf55x25a1ryg6z6hl3lvzw-gettext-0.25.1/include -isystem /nix/store/1dax567dy7cf55x25a1ryg6z6hl3lvzw-gettext-0.25.1/include -isystem /nix/store/1dax567dy7cf55x25a1ryg6z6hl3lvzw-gettext-0.25.1/include -isystem /nix/store/yxi1pmrgn8vsrrncz6pdlxhr5lsc3ysr-glibc-iconv-2.40/include -isystem /nix/store/yxi1pmrgn8vsrrncz6pdlxhr5lsc3ysr-glibc-iconv-2.40/include -isystem /nix/store/yxi1pmrgn8vsrrncz6pdlxhr5lsc3ysr-glibc-iconv-2.40/include -isystem /nix/store/nd2qj4saqgbakhdnpl0f543lj9lalyfv-pango-1.57.0-dev/include -isystem /nix/store/nd2qj4saqgbakhdnpl0f543lj9lalyfv-pango-1.57.0-dev/include -isystem /nix/store/nd2qj4saqgbakhdnpl0f543lj9lalyfv-pango-1.57.0-dev/include -isystem /nix/store/ayhcy9p6rshhda432gc1j3nqbhfklpi2-harfbuzz-12.2.0-dev/include -isystem /nix/store/ayhcy9p6rshhda432gc1j3nqbhfklpi2-harfbuzz-12.2.0-dev/include -isystem /nix/store/ayhcy9p6rshhda432gc1j3nqbhfklpi2-harfbuzz-12.2.0-dev/include -isystem /nix/store/3salm1j8lhcyrkjblp7knj300raadc7g-graphite2-1.3.14-dev/include -isystem /nix/store/3salm1j8lhcyrkjblp7knj300raadc7g-graphite2-1.3.14-dev/include -isystem /nix/store/3salm1j8lhcyrkjblp7knj300raadc7g-graphite2-1.3.14-dev/include -isystem /nix/store/21wibc25j73yapw3si0qbq1cyrv1dyfw-libxft-2.3.9-dev/include -isystem /nix/store/21wibc25j73yapw3si0qbq1cyrv1dyfw-libxft-2.3.9-dev/include -isystem /nix/store/21wibc25j73yapw3si0qbq1cyrv1dyfw-libxft-2.3.9-dev/include -isystem /nix/store/bri9bpcyzlpcawwins7pinrsxrx69y6z-librsvg-2.61.3-dev/include -isystem /nix/store/bri9bpcyzlpcawwins7pinrsxrx69y6z-librsvg-2.61.3-dev/include -isystem /nix/store/bri9bpcyzlpcawwins7pinrsxrx69y6z-librsvg-2.61.3-dev/include -isystem /nix/store/zsa9rgmanx1l9h7i76gfvv440n90vylg-gdk-pixbuf-2.44.4-dev/include -isystem /nix/store/zsa9rgmanx1l9h7i76gfvv440n90vylg-gdk-pixbuf-2.44.4-dev/include -isystem /nix/store/zsa9rgmanx1l9h7i76gfvv440n90vylg-gdk-pixbuf-2.44.4-dev/include -isystem /nix/store/25y7kk3bbscnvvja6bixhz7wa95v2gvi-libtiff-4.7.1-dev/include -isystem /nix/store/25y7kk3bbscnvvja6bixhz7wa95v2gvi-libtiff-4.7.1-dev/include -isystem /nix/store/25y7kk3bbscnvvja6bixhz7wa95v2gvi-libtiff-4.7.1-dev/include -isystem /nix/store/3mxwvjpsqvaairm8b7j495znp6l04j51-vulkan-headers-1.4.335.0/include -isystem /nix/store/3mxwvjpsqvaairm8b7j495znp6l04j51-vulkan-headers-1.4.335.0/include -isystem /nix/store/3mxwvjpsqvaairm8b7j495znp6l04j51-vulkan-headers-1.4.335.0/include -isystem /nix/store/wgjgy1czmbdpq7i1p9kmcyhs5bn3p83c-glm-1.0.2/include -isystem /nix/store/wgjgy1czmbdpq7i1p9kmcyhs5bn3p83c-glm-1.0.2/include -isystem /nix/store/wgjgy1czmbdpq7i1p9kmcyhs5bn3p83c-glm-1.0.2/include -isystem /nix/store/47y76bq8qcnpxizqgl0c0j15s7ciix5r-imagemagick-7.1.2-11-dev/include -isystem /nix/store/47y76bq8qcnpxizqgl0c0j15s7ciix5r-imagemagick-7.1.2-11-dev/include -isystem /nix/store/47y76bq8qcnpxizqgl0c0j15s7ciix5r-imagemagick-7.1.2-11-dev/include -isystem /nix/store/wflnb8fxmq4lqx3ca91hlpv95vrqkv51-curl-8.17.0-dev/include -isystem /nix/store/wflnb8fxmq4lqx3ca91hlpv95vrqkv51-curl-8.17.0-dev/include -isystem /nix/store/wflnb8fxmq4lqx3ca91hlpv95vrqkv51-curl-8.17.0-dev/include -isystem /nix/store/aq62vlf3liwp8rpgn6wp0wsf3wbhcpqb-brotli-1.2.0-dev/include -isystem /nix/store/aq62vlf3liwp8rpgn6wp0wsf3wbhcpqb-brotli-1.2.0-dev/include -isystem /nix/store/aq62vlf3liwp8rpgn6wp0wsf3wbhcpqb-brotli-1.2.0-dev/include -isystem /nix/store/20dygnb42nwy7mbilwgql2lzz8h69pqz-krb5-1.22.1-dev/include -isystem /nix/store/20dygnb42nwy7mbilwgql2lzz8h69pqz-krb5-1.22.1-dev/include -isystem /nix/store/20dygnb42nwy7mbilwgql2lzz8h69pqz-krb5-1.22.1-dev/include -isystem /nix/store/cxvk2hibqamfmgpbz9inbnvfyv49lr0h-nghttp2-1.67.1-dev/include -isystem /nix/store/cxvk2hibqamfmgpbz9inbnvfyv49lr0h-nghttp2-1.67.1-dev/include -isystem /nix/store/cxvk2hibqamfmgpbz9inbnvfyv49lr0h-nghttp2-1.67.1-dev/include -isystem /nix/store/bcqpmsh9w6xdy7yd81nkaij8g3vmd61g-nghttp3-1.13.1-dev/include -isystem /nix/store/bcqpmsh9w6xdy7yd81nkaij8g3vmd61g-nghttp3-1.13.1-dev/include -isystem /nix/store/bcqpmsh9w6xdy7yd81nkaij8g3vmd61g-nghttp3-1.13.1-dev/include -isystem /nix/store/ygwc32xg9dqylv1j3gh6rv2zlrhn4r7h-ngtcp2-1.18.0-dev/include -isystem /nix/store/ygwc32xg9dqylv1j3gh6rv2zlrhn4r7h-ngtcp2-1.18.0-dev/include -isystem /nix/store/ygwc32xg9dqylv1j3gh6rv2zlrhn4r7h-ngtcp2-1.18.0-dev/include -isystem /nix/store/xy9v3ciqbqx2v9vqqn785hk22ndx55yd-libidn2-2.3.8-dev/include -isystem /nix/store/xy9v3ciqbqx2v9vqqn785hk22ndx55yd-libidn2-2.3.8-dev/include -isystem /nix/store/xy9v3ciqbqx2v9vqqn785hk22ndx55yd-libidn2-2.3.8-dev/include -isystem /nix/store/2ivy0r8ab3bnps5957vfrxcjfcgad661-openssl-3.6.0-dev/include -isystem /nix/store/2ivy0r8ab3bnps5957vfrxcjfcgad661-openssl-3.6.0-dev/include -isystem /nix/store/2ivy0r8ab3bnps5957vfrxcjfcgad661-openssl-3.6.0-dev/include -isystem /nix/store/p59c986zyf31f923yhr8sqxdqnq3plnh-libpsl-0.21.5-dev/include -isystem /nix/store/p59c986zyf31f923yhr8sqxdqnq3plnh-libpsl-0.21.5-dev/include -isystem /nix/store/p59c986zyf31f923yhr8sqxdqnq3plnh-libpsl-0.21.5-dev/include -isystem /nix/store/dp3sg21kr75b2059s94g3a2vg8dfajmv-libssh2-1.11.1-dev/include -isystem /nix/store/dp3sg21kr75b2059s94g3a2vg8dfajmv-libssh2-1.11.1-dev/include -isystem /nix/store/dp3sg21kr75b2059s94g3a2vg8dfajmv-libssh2-1.11.1-dev/include -isystem /nix/store/srnvjrr38j7xmpcmjpndd6mw31lsfjrl-zlib-1.3.1-dev/include -isystem /nix/store/srnvjrr38j7xmpcmjpndd6mw31lsfjrl-zlib-1.3.1-dev/include -isystem /nix/store/srnvjrr38j7xmpcmjpndd6mw31lsfjrl-zlib-1.3.1-dev/include -isystem /nix/store/di94mxmhhk390zzzv2gvavfwkgww7q6h-zstd-1.5.7-dev/include -isystem /nix/store/di94mxmhhk390zzzv2gvavfwkgww7q6h-zstd-1.5.7-dev/include -isystem /nix/store/di94mxmhhk390zzzv2gvavfwkgww7q6h-zstd-1.5.7-dev/include -isystem /nix/store/c828rss7q5rra33phplrb12ayv2p9vgw-bzip2-1.0.8-dev/include -isystem /nix/store/c828rss7q5rra33phplrb12ayv2p9vgw-bzip2-1.0.8-dev/include -isystem /nix/store/c828rss7q5rra33phplrb12ayv2p9vgw-bzip2-1.0.8-dev/include -isystem /nix/store/ss8rcralsb3b192371ma7kn9wvl3kdx4-freetype-2.13.3-dev/include -isystem /nix/store/ss8rcralsb3b192371ma7kn9wvl3kdx4-freetype-2.13.3-dev/include -isystem /nix/store/ss8rcralsb3b192371ma7kn9wvl3kdx4-freetype-2.13.3-dev/include -isystem /nix/store/li0sf0lhm30c83s1vi6filranamk9q3c-libpng-apng-1.6.52-dev/include -isystem /nix/store/li0sf0lhm30c83s1vi6filranamk9q3c-libpng-apng-1.6.52-dev/include -isystem /nix/store/li0sf0lhm30c83s1vi6filranamk9q3c-libpng-apng-1.6.52-dev/include -isystem /nix/store/5ljn72xffsivv2ya5ncrzkyf5nx94227-libjpeg-turbo-3.1.3-dev/include -isystem /nix/store/5ljn72xffsivv2ya5ncrzkyf5nx94227-libjpeg-turbo-3.1.3-dev/include -isystem /nix/store/5ljn72xffsivv2ya5ncrzkyf5nx94227-libjpeg-turbo-3.1.3-dev/include -isystem /nix/store/lb33y3zdr3dzc948mf9v8n6sznc5b9bf-lcms2-2.17-dev/include -isystem /nix/store/lb33y3zdr3dzc948mf9v8n6sznc5b9bf-lcms2-2.17-dev/include -isystem /nix/store/lb33y3zdr3dzc948mf9v8n6sznc5b9bf-lcms2-2.17-dev/include -isystem /nix/store/vx5qrqwr1600p7db7ahykcgqbakk7gzc-libx11-1.8.12-dev/include -isystem /nix/store/vx5qrqwr1600p7db7ahykcgqbakk7gzc-libx11-1.8.12-dev/include -isystem /nix/store/vx5qrqwr1600p7db7ahykcgqbakk7gzc-libx11-1.8.12-dev/include -isystem /nix/store/vfvnfnq9xb69gzxqv1s5kbpldi99bzr2-xorgproto-2024.1/include -isystem /nix/store/vfvnfnq9xb69gzxqv1s5kbpldi99bzr2-xorgproto-2024.1/include -isystem /nix/store/vfvnfnq9xb69gzxqv1s5kbpldi99bzr2-xorgproto-2024.1/include -isystem /nix/store/s75hddpdhzzakb6jbzacq2q5bgh2y36k-libxt-1.3.1-dev/include -isystem /nix/store/s75hddpdhzzakb6jbzacq2q5bgh2y36k-libxt-1.3.1-dev/include -isystem /nix/store/s75hddpdhzzakb6jbzacq2q5bgh2y36k-libxt-1.3.1-dev/include -isystem /nix/store/3xf66ysnxf1d5i37fnl144781iyva4q3-libsm-1.2.6-dev/include -isystem /nix/store/3xf66ysnxf1d5i37fnl144781iyva4q3-libsm-1.2.6-dev/include -isystem /nix/store/3xf66ysnxf1d5i37fnl144781iyva4q3-libsm-1.2.6-dev/include -isystem /nix/store/5blg2pq37crjblq81sklbr7imya83wnb-libice-1.1.2-dev/include -isystem /nix/store/5blg2pq37crjblq81sklbr7imya83wnb-libice-1.1.2-dev/include -isystem /nix/store/5blg2pq37crjblq81sklbr7imya83wnb-libice-1.1.2-dev/include -isystem /nix/store/i6742f2vdh0y6k1qyc0m7srk6h7ks0nh-libwebp-1.6.0/include -isystem /nix/store/i6742f2vdh0y6k1qyc0m7srk6h7ks0nh-libwebp-1.6.0/include -isystem /nix/store/i6742f2vdh0y6k1qyc0m7srk6h7ks0nh-libwebp-1.6.0/include -isystem /nix/store/vlcfhfmsm11490mp7fd2n0g9rnapgpid-fftw-double-3.3.10-dev/include -isystem /nix/store/vlcfhfmsm11490mp7fd2n0g9rnapgpid-fftw-double-3.3.10-dev/include -isystem /nix/store/vlcfhfmsm11490mp7fd2n0g9rnapgpid-fftw-double-3.3.10-dev/include -isystem /nix/store/n0cf9vad3va0js6606gcra8xhsnr0js7-gdb-16.3/include -isystem /nix/store/n0cf9vad3va0js6606gcra8xhsnr0js7-gdb-16.3/include -isystem /nix/store/n0cf9vad3va0js6606gcra8xhsnr0js7-gdb-16.3/include -isystem /nix/store/n5m6p0j4anvf5cyy79nb2qn7smkzs176-gnumake-4.4.1/include -isystem /nix/store/n5m6p0j4anvf5cyy79nb2qn7smkzs176-gnumake-4.4.1/include -isystem /nix/store/n5m6p0j4anvf5cyy79nb2qn7smkzs176-gnumake-4.4.1/include -isystem /nix/store/dk4dmm4bvskdjb84qp3yyg45mx3krysv-weston-14.0.1/include -isystem /nix/store/dk4dmm4bvskdjb84qp3yyg45mx3krysv-weston-14.0.1/include -isystem /nix/store/dk4dmm4bvskdjb84qp3yyg45mx3krysv-weston-14.0.1/include -isystem /nix/store/w7rpcgzwhqdcrsxzj3ml4njjwv0bar82-compiler-rt-libc-21.1.7-dev/include -isystem /nix/store/w7rpcgzwhqdcrsxzj3ml4njjwv0bar82-compiler-rt-libc-21.1.7-dev/include -isystem /nix/store/w7rpcgzwhqdcrsxzj3ml4njjwv0bar82-compiler-rt-libc-21.1.7-dev/include -isystem /nix/store/gwyq2ns8nzgf6q3cvr1k8gasfn03khcp-libcxx-21.1.7-dev/include -isystem /nix/store/gwyq2ns8nzgf6q3cvr1k8gasfn03khcp-libcxx-21.1.7-dev/include -isystem /nix/store/gwyq2ns8nzgf6q3cvr1k8gasfn03khcp-libcxx-21.1.7-dev/include -isystem /nix/store/vxm9x8hynlq20jsm38waw7ji3qprd6rq-wlroots-0.19.2/include -isystem /nix/store/vxm9x8hynlq20jsm38waw7ji3qprd6rq-wlroots-0.19.2/include -isystem /nix/store/vxm9x8hynlq20jsm38waw7ji3qprd6rq-wlroots-0.19.2/include -isystem /nix/store/c3q7cp0nbvndl38znhqw3daihnirqj3g-wayland-1.24.0-dev/include -isystem /nix/store/c3q7cp0nbvndl38znhqw3daihnirqj3g-wayland-1.24.0-dev/include -isystem /nix/store/c3q7cp0nbvndl38znhqw3daihnirqj3g-wayland-1.24.0-dev/include -isystem /nix/store/zfl3y2dfih1qff84gldbkda1pz1lg45p-wayland-protocols-1.46/include -isystem /nix/store/zfl3y2dfih1qff84gldbkda1pz1lg45p-wayland-protocols-1.46/include -isystem /nix/store/zfl3y2dfih1qff84gldbkda1pz1lg45p-wayland-protocols-1.46/include -isystem /nix/store/5s110bxdr4jvxi3wjd2dwwjv9inn8ddp-libxkbcommon-1.11.0-dev/include -isystem /nix/store/5s110bxdr4jvxi3wjd2dwwjv9inn8ddp-libxkbcommon-1.11.0-dev/include -isystem /nix/store/5s110bxdr4jvxi3wjd2dwwjv9inn8ddp-libxkbcommon-1.11.0-dev/include -isystem /nix/store/sf0xrxsq0xr2iq96b8awrwwliik5dm6r-libdrm-2.4.129-dev/include -isystem /nix/store/sf0xrxsq0xr2iq96b8awrwwliik5dm6r-libdrm-2.4.129-dev/include -isystem /nix/store/sf0xrxsq0xr2iq96b8awrwwliik5dm6r-libdrm-2.4.129-dev/include -isystem /nix/store/pa3asawv8vx96lz2zsm89rrg13flr4wk-libinput-1.29.2-dev/include -isystem /nix/store/pa3asawv8vx96lz2zsm89rrg13flr4wk-libinput-1.29.2-dev/include -isystem /nix/store/pa3asawv8vx96lz2zsm89rrg13flr4wk-libinput-1.29.2-dev/include -isystem /nix/store/qhw6n40d6mi5ply6rf4gmf6b0vvg1wai-systemd-minimal-libs-258.2-dev/include -isystem /nix/store/qhw6n40d6mi5ply6rf4gmf6b0vvg1wai-systemd-minimal-libs-258.2-dev/include -isystem /nix/store/qhw6n40d6mi5ply6rf4gmf6b0vvg1wai-systemd-minimal-libs-258.2-dev/include -isystem /nix/store/1cnvyk57f3wqlmbpidz0x2824ip9f8ly-pixman-0.46.4/include -isystem /nix/store/1cnvyk57f3wqlmbpidz0x2824ip9f8ly-pixman-0.46.4/include -isystem /nix/store/1cnvyk57f3wqlmbpidz0x2824ip9f8ly-pixman-0.46.4/include -isystem /nix/store/8imqqzx7d62hfbn177lx18kqx08x8bma-seatd-0.9.1-dev/include -isystem /nix/store/8imqqzx7d62hfbn177lx18kqx08x8bma-seatd-0.9.1-dev/include -isystem /nix/store/8imqqzx7d62hfbn177lx18kqx08x8bma-seatd-0.9.1-dev/include -isystem /nix/store/yydc0n1z5ag31x2vqv05n8iz498nvlm2-libglvnd-1.7.0-dev/include -isystem /nix/store/yydc0n1z5ag31x2vqv05n8iz498nvlm2-libglvnd-1.7.0-dev/include -isystem /nix/store/yydc0n1z5ag31x2vqv05n8iz498nvlm2-libglvnd-1.7.0-dev/include -isystem /nix/store/ad2zpdj63hrnl90r01w9573m56gbkrh9-libffi-3.5.2-dev/include -isystem /nix/store/ad2zpdj63hrnl90r01w9573m56gbkrh9-libffi-3.5.2-dev/include -isystem /nix/store/ad2zpdj63hrnl90r01w9573m56gbkrh9-libffi-3.5.2-dev/include -isystem /nix/store/nkgs0z67asdq920jxpkxazinzl11k0d3-libcap-2.77-dev/include -isystem /nix/store/nkgs0z67asdq920jxpkxazinzl11k0d3-libcap-2.77-dev/include -isystem /nix/store/nkgs0z67asdq920jxpkxazinzl11k0d3-libcap-2.77-dev/include -isystem /nix/store/g3xkwyfnh4lifwyx6kmhnpz9bw2540hm-libxcb-1.17.0-dev/include -isystem /nix/store/g3xkwyfnh4lifwyx6kmhnpz9bw2540hm-libxcb-1.17.0-dev/include -isystem /nix/store/g3xkwyfnh4lifwyx6kmhnpz9bw2540hm-libxcb-1.17.0-dev/include -isystem /nix/store/wd6lqkzpll686i5q9snd1c9c7glhcqqi-cairo-1.18.4-dev/include -isystem /nix/store/wd6lqkzpll686i5q9snd1c9c7glhcqqi-cairo-1.18.4-dev/include -isystem /nix/store/wd6lqkzpll686i5q9snd1c9c7glhcqqi-cairo-1.18.4-dev/include -isystem /nix/store/pp2fbp19cabbdb2dhslf3qnxbsyyvnxm-fontconfig-2.17.1-dev/include -isystem /nix/store/pp2fbp19cabbdb2dhslf3qnxbsyyvnxm-fontconfig-2.17.1-dev/include -isystem /nix/store/pp2fbp19cabbdb2dhslf3qnxbsyyvnxm-fontconfig-2.17.1-dev/include -isystem /nix/store/z7yvs4816g5xgmcj8122h0w97y5aa6l0-libxext-1.3.6-dev/include -isystem /nix/store/z7yvs4816g5xgmcj8122h0w97y5aa6l0-libxext-1.3.6-dev/include -isystem /nix/store/z7yvs4816g5xgmcj8122h0w97y5aa6l0-libxext-1.3.6-dev/include -isystem /nix/store/fjp9qs2ww92pyag83fgq7amvqhph1dk7-libxau-1.0.12-dev/include -isystem /nix/store/fjp9qs2ww92pyag83fgq7amvqhph1dk7-libxau-1.0.12-dev/include -isystem /nix/store/fjp9qs2ww92pyag83fgq7amvqhph1dk7-libxau-1.0.12-dev/include -isystem /nix/store/b4vxwmm7dr91x6ihh052k8ksxx5lybmj-libxrender-0.9.12-dev/include -isystem /nix/store/b4vxwmm7dr91x6ihh052k8ksxx5lybmj-libxrender-0.9.12-dev/include -isystem /nix/store/b4vxwmm7dr91x6ihh052k8ksxx5lybmj-libxrender-0.9.12-dev/include -isystem /nix/store/j359ndglk11v5j47ra66lc1vxlk2dbjw-glib-2.86.3-dev/include -isystem /nix/store/j359ndglk11v5j47ra66lc1vxlk2dbjw-glib-2.86.3-dev/include -isystem /nix/store/j359ndglk11v5j47ra66lc1vxlk2dbjw-glib-2.86.3-dev/include -isystem /nix/store/1dax567dy7cf55x25a1ryg6z6hl3lvzw-gettext-0.25.1/include -isystem /nix/store/1dax567dy7cf55x25a1ryg6z6hl3lvzw-gettext-0.25.1/include -isystem /nix/store/1dax567dy7cf55x25a1ryg6z6hl3lvzw-gettext-0.25.1/include -isystem /nix/store/yxi1pmrgn8vsrrncz6pdlxhr5lsc3ysr-glibc-iconv-2.40/include -isystem /nix/store/yxi1pmrgn8vsrrncz6pdlxhr5lsc3ysr-glibc-iconv-2.40/include -isystem /nix/store/yxi1pmrgn8vsrrncz6pdlxhr5lsc3ysr-glibc-iconv-2.40/include -isystem /nix/store/nd2qj4saqgbakhdnpl0f543lj9lalyfv-pango-1.57.0-dev/include -isystem /nix/store/nd2qj4saqgbakhdnpl0f543lj9lalyfv-pango-1.57.0-dev/include -isystem /nix/store/nd2qj4saqgbakhdnpl0f543lj9lalyfv-pango-1.57.0-dev/include -isystem /nix/store/ayhcy9p6rshhda432gc1j3nqbhfklpi2-harfbuzz-12.2.0-dev/include -isystem /nix/store/ayhcy9p6rshhda432gc1j3nqbhfklpi2-harfbuzz-12.2.0-dev/include -isystem /nix/store/ayhcy9p6rshhda432gc1j3nqbhfklpi2-harfbuzz-12.2.0-dev/include -isystem /nix/store/3salm1j8lhcyrkjblp7knj300raadc7g-graphite2-1.3.14-dev/include -isystem /nix/store/3salm1j8lhcyrkjblp7knj300raadc7g-graphite2-1.3.14-dev/include -isystem /nix/store/3salm1j8lhcyrkjblp7knj300raadc7g-graphite2-1.3.14-dev/include -isystem /nix/store/21wibc25j73yapw3si0qbq1cyrv1dyfw-libxft-2.3.9-dev/include -isystem /nix/store/21wibc25j73yapw3si0qbq1cyrv1dyfw-libxft-2.3.9-dev/include -isystem /nix/store/21wibc25j73yapw3si0qbq1cyrv1dyfw-libxft-2.3.9-dev/include -isystem /nix/store/bri9bpcyzlpcawwins7pinrsxrx69y6z-librsvg-2.61.3-dev/include -isystem /nix/store/bri9bpcyzlpcawwins7pinrsxrx69y6z-librsvg-2.61.3-dev/include -isystem /nix/store/bri9bpcyzlpcawwins7pinrsxrx69y6z-librsvg-2.61.3-dev/include -isystem /nix/store/zsa9rgmanx1l9h7i76gfvv440n90vylg-gdk-pixbuf-2.44.4-dev/include -isystem /nix/store/zsa9rgmanx1l9h7i76gfvv440n90vylg-gdk-pixbuf-2.44.4-dev/include -isystem /nix/store/zsa9rgmanx1l9h7i76gfvv440n90vylg-gdk-pixbuf-2.44.4-dev/include -isystem /nix/store/25y7kk3bbscnvvja6bixhz7wa95v2gvi-libtiff-4.7.1-dev/include -isystem /nix/store/25y7kk3bbscnvvja6bixhz7wa95v2gvi-libtiff-4.7.1-dev/include -isystem /nix/store/25y7kk3bbscnvvja6bixhz7wa95v2gvi-libtiff-4.7.1-dev/include -isystem /nix/store/3mxwvjpsqvaairm8b7j495znp6l04j51-vulkan-headers-1.4.335.0/include -isystem /nix/store/3mxwvjpsqvaairm8b7j495znp6l04j51-vulkan-headers-1.4.335.0/include -isystem /nix/store/3mxwvjpsqvaairm8b7j495znp6l04j51-vulkan-headers-1.4.335.0/include -isystem /nix/store/wgjgy1czmbdpq7i1p9kmcyhs5bn3p83c-glm-1.0.2/include -isystem /nix/store/wgjgy1czmbdpq7i1p9kmcyhs5bn3p83c-glm-1.0.2/include -isystem /nix/store/wgjgy1czmbdpq7i1p9kmcyhs5bn3p83c-glm-1.0.2/include'
export NIX_CFLAGS_COMPILE
NIX_ENFORCE_NO_NATIVE='1'
export NIX_ENFORCE_NO_NATIVE
NIX_HARDENING_ENABLE='bindnow format fortify fortify3 libcxxhardeningextensive libcxxhardeningfast pic relro stackclashprotection stackprotector strictoverflow zerocallusedregs'
export NIX_HARDENING_ENABLE
NIX_LDFLAGS='-rpath /mnt/c/Users/hp/Documents/VitusOS-Ares/outputs/out/lib  -L/nix/store/rj1cvkpc8nijyg7mv90c58s4kkhz3fiq-brotli-1.2.0-lib/lib -L/nix/store/rj1cvkpc8nijyg7mv90c58s4kkhz3fiq-brotli-1.2.0-lib/lib -L/nix/store/3n3j314vsy174dnwks8dd6c0vrqvh8h6-krb5-1.22.1-lib/lib -L/nix/store/3n3j314vsy174dnwks8dd6c0vrqvh8h6-krb5-1.22.1-lib/lib -L/nix/store/bhhlhxd7xzvmgyvxhjls4x09xfxfnrwc-nghttp2-1.67.1-lib/lib -L/nix/store/bhhlhxd7xzvmgyvxhjls4x09xfxfnrwc-nghttp2-1.67.1-lib/lib -L/nix/store/qflgvhllyh98p52gh8wjlizfphhzsxb8-nghttp3-1.13.1/lib -L/nix/store/qflgvhllyh98p52gh8wjlizfphhzsxb8-nghttp3-1.13.1/lib -L/nix/store/m0xx7svz6967pw5knhqff8m0f48w0gj0-ngtcp2-1.18.0/lib -L/nix/store/m0xx7svz6967pw5knhqff8m0f48w0gj0-ngtcp2-1.18.0/lib -L/nix/store/kywwgk85nl83mpf10av3bvm2khdlq5ib-libidn2-2.3.8/lib -L/nix/store/kywwgk85nl83mpf10av3bvm2khdlq5ib-libidn2-2.3.8/lib -L/nix/store/rfm5m2l26lqkskcvxn5bm5xqh6c8wqr5-openssl-3.6.0/lib -L/nix/store/rfm5m2l26lqkskcvxn5bm5xqh6c8wqr5-openssl-3.6.0/lib -L/nix/store/yrfd45bhkyv1hbsvl81987ri326qbb3z-libpsl-0.21.5/lib -L/nix/store/yrfd45bhkyv1hbsvl81987ri326qbb3z-libpsl-0.21.5/lib -L/nix/store/mxx1l7d0zhvqd9758rcbz0sbbaimkkq2-libssh2-1.11.1/lib -L/nix/store/mxx1l7d0zhvqd9758rcbz0sbbaimkkq2-libssh2-1.11.1/lib -L/nix/store/c2qsgf2832zi4n29gfkqgkjpvmbmxam6-zlib-1.3.1/lib -L/nix/store/c2qsgf2832zi4n29gfkqgkjpvmbmxam6-zlib-1.3.1/lib -L/nix/store/w07dqm9na06xgf9vbs0xqrlzya9p7g6b-zstd-1.5.7/lib -L/nix/store/w07dqm9na06xgf9vbs0xqrlzya9p7g6b-zstd-1.5.7/lib -L/nix/store/0apq92vpf3f3kll8vl9nhjq1gaaffibk-curl-8.17.0/lib -L/nix/store/0apq92vpf3f3kll8vl9nhjq1gaaffibk-curl-8.17.0/lib -L/nix/store/0c22zivvf0yspdvr2960rvmjgiwwi5wm-bzip2-1.0.8/lib -L/nix/store/0c22zivvf0yspdvr2960rvmjgiwwi5wm-bzip2-1.0.8/lib -L/nix/store/94dcyz2ssdhr0s2srnafpj49ia6shsn1-libpng-apng-1.6.52/lib -L/nix/store/94dcyz2ssdhr0s2srnafpj49ia6shsn1-libpng-apng-1.6.52/lib -L/nix/store/cvckjvajk2pdlx7i573fnd7ycrl46f17-freetype-2.13.3/lib -L/nix/store/cvckjvajk2pdlx7i573fnd7ycrl46f17-freetype-2.13.3/lib -L/nix/store/alddrrr7703gs9lhw7lknzwr51kj4jsl-libjpeg-turbo-3.1.3/lib -L/nix/store/alddrrr7703gs9lhw7lknzwr51kj4jsl-libjpeg-turbo-3.1.3/lib -L/nix/store/b8vsara78lvpyqz25n03qljh0587fqv5-lcms2-2.17/lib -L/nix/store/b8vsara78lvpyqz25n03qljh0587fqv5-lcms2-2.17/lib -L/nix/store/ii3ybky5dqjikcrw7vdnh1j76ssy0ycm-libx11-1.8.12/lib -L/nix/store/ii3ybky5dqjikcrw7vdnh1j76ssy0ycm-libx11-1.8.12/lib -L/nix/store/7yr1yymvbrkqyqxfp6hf0y7k2f8s91c7-libice-1.1.2/lib -L/nix/store/7yr1yymvbrkqyqxfp6hf0y7k2f8s91c7-libice-1.1.2/lib -L/nix/store/s3syj0fs1a4rfi51fvgjmngagfyiiz9i-libsm-1.2.6/lib -L/nix/store/s3syj0fs1a4rfi51fvgjmngagfyiiz9i-libsm-1.2.6/lib -L/nix/store/5lm05s676bd9r6kgqnymbghzp6lg9xyw-libxt-1.3.1/lib -L/nix/store/5lm05s676bd9r6kgqnymbghzp6lg9xyw-libxt-1.3.1/lib -L/nix/store/i6742f2vdh0y6k1qyc0m7srk6h7ks0nh-libwebp-1.6.0/lib -L/nix/store/i6742f2vdh0y6k1qyc0m7srk6h7ks0nh-libwebp-1.6.0/lib -L/nix/store/qpg5bb13x7p13d02kb8rv1f4333glzlw-fftw-double-3.3.10/lib -L/nix/store/qpg5bb13x7p13d02kb8rv1f4333glzlw-fftw-double-3.3.10/lib -L/nix/store/dfj4g2rpz8yf1g6ybj7d37p4kmwsl772-imagemagick-7.1.2-11/lib -L/nix/store/dfj4g2rpz8yf1g6ybj7d37p4kmwsl772-imagemagick-7.1.2-11/lib -L/nix/store/n0cf9vad3va0js6606gcra8xhsnr0js7-gdb-16.3/lib -L/nix/store/n0cf9vad3va0js6606gcra8xhsnr0js7-gdb-16.3/lib -L/nix/store/dk4dmm4bvskdjb84qp3yyg45mx3krysv-weston-14.0.1/lib -L/nix/store/dk4dmm4bvskdjb84qp3yyg45mx3krysv-weston-14.0.1/lib -L/nix/store/9d1kansnki5ipsfk9gv1ajb6fxd218ik-libcxx-21.1.7/lib -L/nix/store/9d1kansnki5ipsfk9gv1ajb6fxd218ik-libcxx-21.1.7/lib -L/nix/store/vxm9x8hynlq20jsm38waw7ji3qprd6rq-wlroots-0.19.2/lib -L/nix/store/vxm9x8hynlq20jsm38waw7ji3qprd6rq-wlroots-0.19.2/lib -L/nix/store/7m6x6b4ljrkmkbhvbxdcacpraxj49xkj-wayland-1.24.0/lib -L/nix/store/7m6x6b4ljrkmkbhvbxdcacpraxj49xkj-wayland-1.24.0/lib -L/nix/store/91mg839gbvgz1svzlh1jvz0pqvx7ylwh-libxkbcommon-1.11.0/lib -L/nix/store/91mg839gbvgz1svzlh1jvz0pqvx7ylwh-libxkbcommon-1.11.0/lib -L/nix/store/sixic8a4adxz72swww4fcls0h5sabjdf-libdrm-2.4.129/lib -L/nix/store/sixic8a4adxz72swww4fcls0h5sabjdf-libdrm-2.4.129/lib -L/nix/store/n234iswy40dzhd7aw6ak0wfrbyjpvbw4-systemd-minimal-libs-258.2/lib -L/nix/store/n234iswy40dzhd7aw6ak0wfrbyjpvbw4-systemd-minimal-libs-258.2/lib -L/nix/store/pfaavvrfb1lhx26zwllc4vm5jyzfl1hh-libinput-1.29.2/lib -L/nix/store/pfaavvrfb1lhx26zwllc4vm5jyzfl1hh-libinput-1.29.2/lib -L/nix/store/1cnvyk57f3wqlmbpidz0x2824ip9f8ly-pixman-0.46.4/lib -L/nix/store/1cnvyk57f3wqlmbpidz0x2824ip9f8ly-pixman-0.46.4/lib -L/nix/store/zjqacif3aa0idg3k18dlsz37iyzn0fqm-seatd-0.9.1/lib -L/nix/store/zjqacif3aa0idg3k18dlsz37iyzn0fqm-seatd-0.9.1/lib -L/nix/store/x4sc437gdbhrfbm4b0hja9lfv38njvi1-libglvnd-1.7.0/lib -L/nix/store/x4sc437gdbhrfbm4b0hja9lfv38njvi1-libglvnd-1.7.0/lib -L/nix/store/638y6yx9m2f5il7azqy1fn3hcczwjvfx-mesa-25.3.3/lib -L/nix/store/638y6yx9m2f5il7azqy1fn3hcczwjvfx-mesa-25.3.3/lib -L/nix/store/1z031fwbsc8jhmm7i39r6z6m1xn7rbza-libffi-3.5.2/lib -L/nix/store/1z031fwbsc8jhmm7i39r6z6m1xn7rbza-libffi-3.5.2/lib -L/nix/store/n16jmmckg4fpqk97542hbdvlryxray79-libcap-2.77-lib/lib -L/nix/store/n16jmmckg4fpqk97542hbdvlryxray79-libcap-2.77-lib/lib -L/nix/store/m0jzvlr5gl7zvdn0fr7c09mjwx65fx58-libxcb-1.17.0/lib -L/nix/store/m0jzvlr5gl7zvdn0fr7c09mjwx65fx58-libxcb-1.17.0/lib -L/nix/store/va64srrrxljdrkh4w7y97gl3rzj6dgj9-fontconfig-2.17.1-lib/lib -L/nix/store/va64srrrxljdrkh4w7y97gl3rzj6dgj9-fontconfig-2.17.1-lib/lib -L/nix/store/whgxask7xm93jimlqrc2gm3j46p038gi-libxau-1.0.12/lib -L/nix/store/whgxask7xm93jimlqrc2gm3j46p038gi-libxau-1.0.12/lib -L/nix/store/zshby6nalhw4mvap0rr97hv042808c2k-libxext-1.3.6/lib -L/nix/store/zshby6nalhw4mvap0rr97hv042808c2k-libxext-1.3.6/lib -L/nix/store/8xmc6wk01sgy6djqchvwf168vk6sp0qa-libxrender-0.9.12/lib -L/nix/store/8xmc6wk01sgy6djqchvwf168vk6sp0qa-libxrender-0.9.12/lib -L/nix/store/1dax567dy7cf55x25a1ryg6z6hl3lvzw-gettext-0.25.1/lib -L/nix/store/1dax567dy7cf55x25a1ryg6z6hl3lvzw-gettext-0.25.1/lib -L/nix/store/f7rcazhd826xlcz43il4vafv28888cgj-glib-2.86.3/lib -L/nix/store/f7rcazhd826xlcz43il4vafv28888cgj-glib-2.86.3/lib -L/nix/store/733dh2bc55l8khcnkanmz1f99m77xjm0-cairo-1.18.4/lib -L/nix/store/733dh2bc55l8khcnkanmz1f99m77xjm0-cairo-1.18.4/lib -L/nix/store/7n6bbv5ll4wpjq1kfbch7b16s74my77y-graphite2-1.3.14/lib -L/nix/store/7n6bbv5ll4wpjq1kfbch7b16s74my77y-graphite2-1.3.14/lib -L/nix/store/40b7x2n2yylv79rgnavw3xk7yp9308jx-harfbuzz-12.2.0/lib -L/nix/store/40b7x2n2yylv79rgnavw3xk7yp9308jx-harfbuzz-12.2.0/lib -L/nix/store/wbnn8hmnfcd7pmk0n18fr7azsqrml1d0-libxft-2.3.9/lib -L/nix/store/wbnn8hmnfcd7pmk0n18fr7azsqrml1d0-libxft-2.3.9/lib -L/nix/store/7qlq78jh6n35v1xpxyd6c279nbf0r88f-pango-1.57.0/lib -L/nix/store/7qlq78jh6n35v1xpxyd6c279nbf0r88f-pango-1.57.0/lib -L/nix/store/n1276dzpdi463s5nb2fsmysnxkirn3j6-libtiff-4.7.1/lib -L/nix/store/n1276dzpdi463s5nb2fsmysnxkirn3j6-libtiff-4.7.1/lib -L/nix/store/5k9nfx38ckb0wkwsgy925ddzdhrvj16s-gdk-pixbuf-2.44.4/lib -L/nix/store/5k9nfx38ckb0wkwsgy925ddzdhrvj16s-gdk-pixbuf-2.44.4/lib -L/nix/store/8gz43a07r76vahgqlx88iz9yi22shdv0-librsvg-2.61.3/lib -L/nix/store/8gz43a07r76vahgqlx88iz9yi22shdv0-librsvg-2.61.3/lib -L/nix/store/jzng5i2xwsdlf3h60a5s6qzr2808c9zc-vulkan-loader-1.4.335.0/lib -L/nix/store/jzng5i2xwsdlf3h60a5s6qzr2808c9zc-vulkan-loader-1.4.335.0/lib -L/nix/store/94i9dxdbxpf6fyxsf7gviscnk5fs844x-vulkan-validation-layers-1.4.335.0/lib -L/nix/store/94i9dxdbxpf6fyxsf7gviscnk5fs844x-vulkan-validation-layers-1.4.335.0/lib -L/nix/store/wgjgy1czmbdpq7i1p9kmcyhs5bn3p83c-glm-1.0.2/lib -L/nix/store/wgjgy1czmbdpq7i1p9kmcyhs5bn3p83c-glm-1.0.2/lib -L/nix/store/rj1cvkpc8nijyg7mv90c58s4kkhz3fiq-brotli-1.2.0-lib/lib -L/nix/store/rj1cvkpc8nijyg7mv90c58s4kkhz3fiq-brotli-1.2.0-lib/lib -L/nix/store/3n3j314vsy174dnwks8dd6c0vrqvh8h6-krb5-1.22.1-lib/lib -L/nix/store/3n3j314vsy174dnwks8dd6c0vrqvh8h6-krb5-1.22.1-lib/lib -L/nix/store/bhhlhxd7xzvmgyvxhjls4x09xfxfnrwc-nghttp2-1.67.1-lib/lib -L/nix/store/bhhlhxd7xzvmgyvxhjls4x09xfxfnrwc-nghttp2-1.67.1-lib/lib -L/nix/store/qflgvhllyh98p52gh8wjlizfphhzsxb8-nghttp3-1.13.1/lib -L/nix/store/qflgvhllyh98p52gh8wjlizfphhzsxb8-nghttp3-1.13.1/lib -L/nix/store/m0xx7svz6967pw5knhqff8m0f48w0gj0-ngtcp2-1.18.0/lib -L/nix/store/m0xx7svz6967pw5knhqff8m0f48w0gj0-ngtcp2-1.18.0/lib -L/nix/store/kywwgk85nl83mpf10av3bvm2khdlq5ib-libidn2-2.3.8/lib -L/nix/store/kywwgk85nl83mpf10av3bvm2khdlq5ib-libidn2-2.3.8/lib -L/nix/store/rfm5m2l26lqkskcvxn5bm5xqh6c8wqr5-openssl-3.6.0/lib -L/nix/store/rfm5m2l26lqkskcvxn5bm5xqh6c8wqr5-openssl-3.6.0/lib -L/nix/store/yrfd45bhkyv1hbsvl81987ri326qbb3z-libpsl-0.21.5/lib -L/nix/store/yrfd45bhkyv1hbsvl81987ri326qbb3z-libpsl-0.21.5/lib -L/nix/store/mxx1l7d0zhvqd9758rcbz0sbbaimkkq2-libssh2-1.11.1/lib -L/nix/store/mxx1l7d0zhvqd9758rcbz0sbbaimkkq2-libssh2-1.11.1/lib -L/nix/store/c2qsgf2832zi4n29gfkqgkjpvmbmxam6-zlib-1.3.1/lib -L/nix/store/c2qsgf2832zi4n29gfkqgkjpvmbmxam6-zlib-1.3.1/lib -L/nix/store/w07dqm9na06xgf9vbs0xqrlzya9p7g6b-zstd-1.5.7/lib -L/nix/store/w07dqm9na06xgf9vbs0xqrlzya9p7g6b-zstd-1.5.7/lib -L/nix/store/0apq92vpf3f3kll8vl9nhjq1gaaffibk-curl-8.17.0/lib -L/nix/store/0apq92vpf3f3kll8vl9nhjq1gaaffibk-curl-8.17.0/lib -L/nix/store/0c22zivvf0yspdvr2960rvmjgiwwi5wm-bzip2-1.0.8/lib -L/nix/store/0c22zivvf0yspdvr2960rvmjgiwwi5wm-bzip2-1.0.8/lib -L/nix/store/94dcyz2ssdhr0s2srnafpj49ia6shsn1-libpng-apng-1.6.52/lib -L/nix/store/94dcyz2ssdhr0s2srnafpj49ia6shsn1-libpng-apng-1.6.52/lib -L/nix/store/cvckjvajk2pdlx7i573fnd7ycrl46f17-freetype-2.13.3/lib -L/nix/store/cvckjvajk2pdlx7i573fnd7ycrl46f17-freetype-2.13.3/lib -L/nix/store/alddrrr7703gs9lhw7lknzwr51kj4jsl-libjpeg-turbo-3.1.3/lib -L/nix/store/alddrrr7703gs9lhw7lknzwr51kj4jsl-libjpeg-turbo-3.1.3/lib -L/nix/store/b8vsara78lvpyqz25n03qljh0587fqv5-lcms2-2.17/lib -L/nix/store/b8vsara78lvpyqz25n03qljh0587fqv5-lcms2-2.17/lib -L/nix/store/ii3ybky5dqjikcrw7vdnh1j76ssy0ycm-libx11-1.8.12/lib -L/nix/store/ii3ybky5dqjikcrw7vdnh1j76ssy0ycm-libx11-1.8.12/lib -L/nix/store/7yr1yymvbrkqyqxfp6hf0y7k2f8s91c7-libice-1.1.2/lib -L/nix/store/7yr1yymvbrkqyqxfp6hf0y7k2f8s91c7-libice-1.1.2/lib -L/nix/store/s3syj0fs1a4rfi51fvgjmngagfyiiz9i-libsm-1.2.6/lib -L/nix/store/s3syj0fs1a4rfi51fvgjmngagfyiiz9i-libsm-1.2.6/lib -L/nix/store/5lm05s676bd9r6kgqnymbghzp6lg9xyw-libxt-1.3.1/lib -L/nix/store/5lm05s676bd9r6kgqnymbghzp6lg9xyw-libxt-1.3.1/lib -L/nix/store/i6742f2vdh0y6k1qyc0m7srk6h7ks0nh-libwebp-1.6.0/lib -L/nix/store/i6742f2vdh0y6k1qyc0m7srk6h7ks0nh-libwebp-1.6.0/lib -L/nix/store/qpg5bb13x7p13d02kb8rv1f4333glzlw-fftw-double-3.3.10/lib -L/nix/store/qpg5bb13x7p13d02kb8rv1f4333glzlw-fftw-double-3.3.10/lib -L/nix/store/dfj4g2rpz8yf1g6ybj7d37p4kmwsl772-imagemagick-7.1.2-11/lib -L/nix/store/dfj4g2rpz8yf1g6ybj7d37p4kmwsl772-imagemagick-7.1.2-11/lib -L/nix/store/n0cf9vad3va0js6606gcra8xhsnr0js7-gdb-16.3/lib -L/nix/store/n0cf9vad3va0js6606gcra8xhsnr0js7-gdb-16.3/lib -L/nix/store/dk4dmm4bvskdjb84qp3yyg45mx3krysv-weston-14.0.1/lib -L/nix/store/dk4dmm4bvskdjb84qp3yyg45mx3krysv-weston-14.0.1/lib -L/nix/store/9d1kansnki5ipsfk9gv1ajb6fxd218ik-libcxx-21.1.7/lib -L/nix/store/9d1kansnki5ipsfk9gv1ajb6fxd218ik-libcxx-21.1.7/lib -L/nix/store/vxm9x8hynlq20jsm38waw7ji3qprd6rq-wlroots-0.19.2/lib -L/nix/store/vxm9x8hynlq20jsm38waw7ji3qprd6rq-wlroots-0.19.2/lib -L/nix/store/7m6x6b4ljrkmkbhvbxdcacpraxj49xkj-wayland-1.24.0/lib -L/nix/store/7m6x6b4ljrkmkbhvbxdcacpraxj49xkj-wayland-1.24.0/lib -L/nix/store/91mg839gbvgz1svzlh1jvz0pqvx7ylwh-libxkbcommon-1.11.0/lib -L/nix/store/91mg839gbvgz1svzlh1jvz0pqvx7ylwh-libxkbcommon-1.11.0/lib -L/nix/store/sixic8a4adxz72swww4fcls0h5sabjdf-libdrm-2.4.129/lib -L/nix/store/sixic8a4adxz72swww4fcls0h5sabjdf-libdrm-2.4.129/lib -L/nix/store/n234iswy40dzhd7aw6ak0wfrbyjpvbw4-systemd-minimal-libs-258.2/lib -L/nix/store/n234iswy40dzhd7aw6ak0wfrbyjpvbw4-systemd-minimal-libs-258.2/lib -L/nix/store/pfaavvrfb1lhx26zwllc4vm5jyzfl1hh-libinput-1.29.2/lib -L/nix/store/pfaavvrfb1lhx26zwllc4vm5jyzfl1hh-libinput-1.29.2/lib -L/nix/store/1cnvyk57f3wqlmbpidz0x2824ip9f8ly-pixman-0.46.4/lib -L/nix/store/1cnvyk57f3wqlmbpidz0x2824ip9f8ly-pixman-0.46.4/lib -L/nix/store/zjqacif3aa0idg3k18dlsz37iyzn0fqm-seatd-0.9.1/lib -L/nix/store/zjqacif3aa0idg3k18dlsz37iyzn0fqm-seatd-0.9.1/lib -L/nix/store/x4sc437gdbhrfbm4b0hja9lfv38njvi1-libglvnd-1.7.0/lib -L/nix/store/x4sc437gdbhrfbm4b0hja9lfv38njvi1-libglvnd-1.7.0/lib -L/nix/store/638y6yx9m2f5il7azqy1fn3hcczwjvfx-mesa-25.3.3/lib -L/nix/store/638y6yx9m2f5il7azqy1fn3hcczwjvfx-mesa-25.3.3/lib -L/nix/store/1z031fwbsc8jhmm7i39r6z6m1xn7rbza-libffi-3.5.2/lib -L/nix/store/1z031fwbsc8jhmm7i39r6z6m1xn7rbza-libffi-3.5.2/lib -L/nix/store/n16jmmckg4fpqk97542hbdvlryxray79-libcap-2.77-lib/lib -L/nix/store/n16jmmckg4fpqk97542hbdvlryxray79-libcap-2.77-lib/lib -L/nix/store/m0jzvlr5gl7zvdn0fr7c09mjwx65fx58-libxcb-1.17.0/lib -L/nix/store/m0jzvlr5gl7zvdn0fr7c09mjwx65fx58-libxcb-1.17.0/lib -L/nix/store/va64srrrxljdrkh4w7y97gl3rzj6dgj9-fontconfig-2.17.1-lib/lib -L/nix/store/va64srrrxljdrkh4w7y97gl3rzj6dgj9-fontconfig-2.17.1-lib/lib -L/nix/store/whgxask7xm93jimlqrc2gm3j46p038gi-libxau-1.0.12/lib -L/nix/store/whgxask7xm93jimlqrc2gm3j46p038gi-libxau-1.0.12/lib -L/nix/store/zshby6nalhw4mvap0rr97hv042808c2k-libxext-1.3.6/lib -L/nix/store/zshby6nalhw4mvap0rr97hv042808c2k-libxext-1.3.6/lib -L/nix/store/8xmc6wk01sgy6djqchvwf168vk6sp0qa-libxrender-0.9.12/lib -L/nix/store/8xmc6wk01sgy6djqchvwf168vk6sp0qa-libxrender-0.9.12/lib -L/nix/store/1dax567dy7cf55x25a1ryg6z6hl3lvzw-gettext-0.25.1/lib -L/nix/store/1dax567dy7cf55x25a1ryg6z6hl3lvzw-gettext-0.25.1/lib -L/nix/store/f7rcazhd826xlcz43il4vafv28888cgj-glib-2.86.3/lib -L/nix/store/f7rcazhd826xlcz43il4vafv28888cgj-glib-2.86.3/lib -L/nix/store/733dh2bc55l8khcnkanmz1f99m77xjm0-cairo-1.18.4/lib -L/nix/store/733dh2bc55l8khcnkanmz1f99m77xjm0-cairo-1.18.4/lib -L/nix/store/7n6bbv5ll4wpjq1kfbch7b16s74my77y-graphite2-1.3.14/lib -L/nix/store/7n6bbv5ll4wpjq1kfbch7b16s74my77y-graphite2-1.3.14/lib -L/nix/store/40b7x2n2yylv79rgnavw3xk7yp9308jx-harfbuzz-12.2.0/lib -L/nix/store/40b7x2n2yylv79rgnavw3xk7yp9308jx-harfbuzz-12.2.0/lib -L/nix/store/wbnn8hmnfcd7pmk0n18fr7azsqrml1d0-libxft-2.3.9/lib -L/nix/store/wbnn8hmnfcd7pmk0n18fr7azsqrml1d0-libxft-2.3.9/lib -L/nix/store/7qlq78jh6n35v1xpxyd6c279nbf0r88f-pango-1.57.0/lib -L/nix/store/7qlq78jh6n35v1xpxyd6c279nbf0r88f-pango-1.57.0/lib -L/nix/store/n1276dzpdi463s5nb2fsmysnxkirn3j6-libtiff-4.7.1/lib -L/nix/store/n1276dzpdi463s5nb2fsmysnxkirn3j6-libtiff-4.7.1/lib -L/nix/store/5k9nfx38ckb0wkwsgy925ddzdhrvj16s-gdk-pixbuf-2.44.4/lib -L/nix/store/5k9nfx38ckb0wkwsgy925ddzdhrvj16s-gdk-pixbuf-2.44.4/lib -L/nix/store/8gz43a07r76vahgqlx88iz9yi22shdv0-librsvg-2.61.3/lib -L/nix/store/8gz43a07r76vahgqlx88iz9yi22shdv0-librsvg-2.61.3/lib -L/nix/store/jzng5i2xwsdlf3h60a5s6qzr2808c9zc-vulkan-loader-1.4.335.0/lib -L/nix/store/jzng5i2xwsdlf3h60a5s6qzr2808c9zc-vulkan-loader-1.4.335.0/lib -L/nix/store/94i9dxdbxpf6fyxsf7gviscnk5fs844x-vulkan-validation-layers-1.4.335.0/lib -L/nix/store/94i9dxdbxpf6fyxsf7gviscnk5fs844x-vulkan-validation-layers-1.4.335.0/lib -L/nix/store/wgjgy1czmbdpq7i1p9kmcyhs5bn3p83c-glm-1.0.2/lib -L/nix/store/wgjgy1czmbdpq7i1p9kmcyhs5bn3p83c-glm-1.0.2/lib'
export NIX_LDFLAGS
NIX_NO_SELF_RPATH='1'
NIX_PKG_CONFIG_WRAPPER_TARGET_HOST_x86_64_unknown_linux_gnu='1'
export NIX_PKG_CONFIG_WRAPPER_TARGET_HOST_x86_64_unknown_linux_gnu
NIX_STORE='/nix/store'
export NIX_STORE
NM='nm'
export NM
OBJCOPY='objcopy'
export OBJCOPY
OBJDUMP='objdump'
export OBJDUMP
OLDPWD=''
export OLDPWD
OPTERR='1'
OSTYPE='linux-gnu'
PATH='/nix/store/wmb16s8hw1vd25vvidwnncpzq126lcz3-cmake-4.1.2/bin:/nix/store/4vxdgiak2ab3s853wz18wj11rmzw8bv6-ninja-1.13.2/bin:/nix/store/hxn2qrz1zmk5q01wsb7n3d58brzrsizb-pkg-config-wrapper-0.29.2/bin:/nix/store/ks51cz85rdrj4a374rfilgxf8b4c8d88-wayland-scanner-1.24.0-bin/bin:/nix/store/47y76bq8qcnpxizqgl0c0j15s7ciix5r-imagemagick-7.1.2-11-dev/bin:/nix/store/wflnb8fxmq4lqx3ca91hlpv95vrqkv51-curl-8.17.0-dev/bin:/nix/store/sk8x7aabysn1wr89i6a813ffii4x7i8w-brotli-1.2.0/bin:/nix/store/20dygnb42nwy7mbilwgql2lzz8h69pqz-krb5-1.22.1-dev/bin:/nix/store/26f9s2k3cx2iri2rjh229na2p5xfmhmb-krb5-1.22.1/bin:/nix/store/r7l6n43ifx8c37qg4wckdms8w6pdlka8-nghttp2-1.67.1/bin:/nix/store/wpq7nwgwlic1yiwnsm5b3brkig3nn50z-libidn2-2.3.8-bin/bin:/nix/store/827b5xq1ghbbdrp1faa4fw32xj1m2p51-openssl-3.6.0-bin/bin:/nix/store/yrfd45bhkyv1hbsvl81987ri326qbb3z-libpsl-0.21.5/bin:/nix/store/4i4q6abxjxnwf4j0ksc2zcb7byaxnrxi-zstd-1.5.7-bin/bin:/nix/store/w07dqm9na06xgf9vbs0xqrlzya9p7g6b-zstd-1.5.7/bin:/nix/store/cr9yz1bdfs9vj9cpr85ihrkgzgawgcwa-curl-8.17.0-bin/bin:/nix/store/p61ba9fdgx3358bpp18hv4rslf6n5bq6-bzip2-1.0.8-bin/bin:/nix/store/ss8rcralsb3b192371ma7kn9wvl3kdx4-freetype-2.13.3-dev/bin:/nix/store/li0sf0lhm30c83s1vi6filranamk9q3c-libpng-apng-1.6.52-dev/bin:/nix/store/35bk8ab4h86q4ymq0irz8ivy3m8q33hf-libjpeg-turbo-3.1.3-bin/bin:/nix/store/a6q5zzy75h8k8wa7mnzqcpvnnr1wj8jy-lcms2-2.17-bin/bin:/nix/store/i6742f2vdh0y6k1qyc0m7srk6h7ks0nh-libwebp-1.6.0/bin:/nix/store/vlcfhfmsm11490mp7fd2n0g9rnapgpid-fftw-double-3.3.10-dev/bin:/nix/store/dfj4g2rpz8yf1g6ybj7d37p4kmwsl772-imagemagick-7.1.2-11/bin:/nix/store/351bpjcf2l1n4vm06nwpq3cdhl6vbhx1-clang-wrapper-21.1.7/bin:/nix/store/mygpiwrw6b4nxab8vb0rmh9xnzmp4w91-clang-21.1.7/bin:/nix/store/7ri1mm5y99nkr7657r77wycrvhg7z9x0-glibc-2.40-66-bin/bin:/nix/store/d75200gb22v7p0703h5jrkgg8bqydk5q-coreutils-9.8/bin:/nix/store/n21wcnpiydy3fd6kqxkwnzvwvaibmg11-binutils-wrapper-2.44/bin:/nix/store/cl88v2m1y5q3k6jlkq5jjf73nmfgl1px-binutils-2.44/bin:/nix/store/8ssdsraa07i1ciyl3az65xqnv44dfmx7-clang-wrapper-21.1.7/bin:/nix/store/n0cf9vad3va0js6606gcra8xhsnr0js7-gdb-16.3/bin:/nix/store/n5m6p0j4anvf5cyy79nb2qn7smkzs176-gnumake-4.4.1/bin:/nix/store/fryiyyhhilpqhx2wc4b6l5gnxc8w18rg-git-2.52.0/bin:/nix/store/dk4dmm4bvskdjb84qp3yyg45mx3krysv-weston-14.0.1/bin:/nix/store/3vs2fr2mazafcdwyza15bfhpmccx1k7z-patchelf-0.15.2/bin:/nix/store/r9wbjib6xxjkyb9yvjvrkl4sq61i2lyn-gcc-wrapper-15.2.0/bin:/nix/store/qarrb8yfby1yyypm32vabzgxgq3w41ma-gcc-15.2.0/bin:/nix/store/dwiyp91lmxq864plaas14jm14m87sg3f-binutils-wrapper-2.44/bin:/nix/store/3mdkalklfdwfdwdi19x1zzlzl017y5xl-compiler-rt-libc-21.1.7/bin:/nix/store/91mg839gbvgz1svzlh1jvz0pqvx7ylwh-libxkbcommon-1.11.0/bin:/nix/store/0j7wz5lhxzzjnq637xyjv9arq6irplks-libdrm-2.4.129-bin/bin:/nix/store/248zkd6222jr7xv5jlwrdj70c15g3qvr-libinput-1.29.2-bin/bin:/nix/store/dyqhp6gciwzm31k2pc8g92rylqk4rfsz-seatd-0.9.1-bin/bin:/nix/store/638y6yx9m2f5il7azqy1fn3hcczwjvfx-mesa-25.3.3/bin:/nix/store/cm1jl79is5qb7zmqg8b8nmrdd7nqdjpk-libcap-2.77/bin:/nix/store/kg9a0vd3kq8p5hx40yw56z07v3vdlpif-xwayland-24.1.9/bin:/nix/store/wd6lqkzpll686i5q9snd1c9c7glhcqqi-cairo-1.18.4-dev/bin:/nix/store/5pzrv0bbcpw6pq4wi6vhjjv597ki2346-fontconfig-2.17.1-bin/bin:/nix/store/j359ndglk11v5j47ra66lc1vxlk2dbjw-glib-2.86.3-dev/bin:/nix/store/1dax567dy7cf55x25a1ryg6z6hl3lvzw-gettext-0.25.1/bin:/nix/store/4c9rdn3nsq3lv44zqsbb865fm174sxx9-glib-2.86.3-bin/bin:/nix/store/ayhcy9p6rshhda432gc1j3nqbhfklpi2-harfbuzz-12.2.0-dev/bin:/nix/store/7n6bbv5ll4wpjq1kfbch7b16s74my77y-graphite2-1.3.14/bin:/nix/store/qn4xlakafmwk72x5dj4bhlgssxim4xf0-pango-1.57.0-bin/bin:/nix/store/zsa9rgmanx1l9h7i76gfvv440n90vylg-gdk-pixbuf-2.44.4-dev/bin:/nix/store/7wg39dxpf0b4vigv4aib7hzqf6134la1-libtiff-4.7.1-bin/bin:/nix/store/5k9nfx38ckb0wkwsgy925ddzdhrvj16s-gdk-pixbuf-2.44.4/bin:/nix/store/8gz43a07r76vahgqlx88iz9yi22shdv0-librsvg-2.61.3/bin:/nix/store/mc8i2kda3zpvfmz9mci7s1dsp3x5q448-vulkan-tools-1.4.335/bin:/nix/store/d75200gb22v7p0703h5jrkgg8bqydk5q-coreutils-9.8/bin:/nix/store/wd99g2j010fdkry0ws1bhxzm52w82ssx-findutils-4.10.0/bin:/nix/store/20zvyjvxq9x2mkp7rbnvrwjjzq2n76hh-diffutils-3.12/bin:/nix/store/k06ssckzrzn9jjvvs4n62m6567zmbx6x-gnused-4.9/bin:/nix/store/qfmq7p42ak5yn389qvx7zpxkan5i4xiy-gnugrep-3.12/bin:/nix/store/nmxm04dhkaqg1q6hai70n9zmzb0q49a5-gawk-5.3.2/bin:/nix/store/k1lcfin159706lihwx5hhvl80dbij4jw-gnutar-1.35/bin:/nix/store/0hv4z5s3r1h4lmvn0427mlxjxgvg34nr-gzip-1.14/bin:/nix/store/p61ba9fdgx3358bpp18hv4rslf6n5bq6-bzip2-1.0.8-bin/bin:/nix/store/mkm3my2067305hdh7rzmi10npwr7y17f-gnumake-4.4.1/bin:/nix/store/lw117lsr8d585xs63kx5k233impyrq7q-bash-5.3p3/bin:/nix/store/clrf4mjwr8xcfpvway6w34wzvqc1hry4-patch-2.8/bin:/nix/store/j1zc5jh0vi9sbxj09ldj4xklgm6kpf8n-xz-5.8.1-bin/bin:/nix/store/s2k48fw3y698j4kcvmw0520m06ihv2z4-file-5.45/bin'
export PATH
PKG_CONFIG='pkg-config'
export PKG_CONFIG
PKG_CONFIG_PATH='/nix/store/6kc4z3z8x47dhxzbha5dkywp5qwji1y7-wayland-scanner-1.24.0-dev/lib/pkgconfig:/nix/store/47y76bq8qcnpxizqgl0c0j15s7ciix5r-imagemagick-7.1.2-11-dev/lib/pkgconfig:/nix/store/wflnb8fxmq4lqx3ca91hlpv95vrqkv51-curl-8.17.0-dev/lib/pkgconfig:/nix/store/aq62vlf3liwp8rpgn6wp0wsf3wbhcpqb-brotli-1.2.0-dev/lib/pkgconfig:/nix/store/20dygnb42nwy7mbilwgql2lzz8h69pqz-krb5-1.22.1-dev/lib/pkgconfig:/nix/store/cxvk2hibqamfmgpbz9inbnvfyv49lr0h-nghttp2-1.67.1-dev/lib/pkgconfig:/nix/store/bcqpmsh9w6xdy7yd81nkaij8g3vmd61g-nghttp3-1.13.1-dev/lib/pkgconfig:/nix/store/ygwc32xg9dqylv1j3gh6rv2zlrhn4r7h-ngtcp2-1.18.0-dev/lib/pkgconfig:/nix/store/xy9v3ciqbqx2v9vqqn785hk22ndx55yd-libidn2-2.3.8-dev/lib/pkgconfig:/nix/store/2ivy0r8ab3bnps5957vfrxcjfcgad661-openssl-3.6.0-dev/lib/pkgconfig:/nix/store/p59c986zyf31f923yhr8sqxdqnq3plnh-libpsl-0.21.5-dev/lib/pkgconfig:/nix/store/dp3sg21kr75b2059s94g3a2vg8dfajmv-libssh2-1.11.1-dev/lib/pkgconfig:/nix/store/srnvjrr38j7xmpcmjpndd6mw31lsfjrl-zlib-1.3.1-dev/lib/pkgconfig:/nix/store/di94mxmhhk390zzzv2gvavfwkgww7q6h-zstd-1.5.7-dev/lib/pkgconfig:/nix/store/c828rss7q5rra33phplrb12ayv2p9vgw-bzip2-1.0.8-dev/lib/pkgconfig:/nix/store/ss8rcralsb3b192371ma7kn9wvl3kdx4-freetype-2.13.3-dev/lib/pkgconfig:/nix/store/li0sf0lhm30c83s1vi6filranamk9q3c-libpng-apng-1.6.52-dev/lib/pkgconfig:/nix/store/5ljn72xffsivv2ya5ncrzkyf5nx94227-libjpeg-turbo-3.1.3-dev/lib/pkgconfig:/nix/store/lb33y3zdr3dzc948mf9v8n6sznc5b9bf-lcms2-2.17-dev/lib/pkgconfig:/nix/store/vx5qrqwr1600p7db7ahykcgqbakk7gzc-libx11-1.8.12-dev/lib/pkgconfig:/nix/store/vfvnfnq9xb69gzxqv1s5kbpldi99bzr2-xorgproto-2024.1/share/pkgconfig:/nix/store/s75hddpdhzzakb6jbzacq2q5bgh2y36k-libxt-1.3.1-dev/lib/pkgconfig:/nix/store/3xf66ysnxf1d5i37fnl144781iyva4q3-libsm-1.2.6-dev/lib/pkgconfig:/nix/store/5blg2pq37crjblq81sklbr7imya83wnb-libice-1.1.2-dev/lib/pkgconfig:/nix/store/i6742f2vdh0y6k1qyc0m7srk6h7ks0nh-libwebp-1.6.0/lib/pkgconfig:/nix/store/vlcfhfmsm11490mp7fd2n0g9rnapgpid-fftw-double-3.3.10-dev/lib/pkgconfig:/nix/store/dk4dmm4bvskdjb84qp3yyg45mx3krysv-weston-14.0.1/lib/pkgconfig:/nix/store/dk4dmm4bvskdjb84qp3yyg45mx3krysv-weston-14.0.1/share/pkgconfig:/nix/store/vxm9x8hynlq20jsm38waw7ji3qprd6rq-wlroots-0.19.2/lib/pkgconfig:/nix/store/c3q7cp0nbvndl38znhqw3daihnirqj3g-wayland-1.24.0-dev/lib/pkgconfig:/nix/store/zfl3y2dfih1qff84gldbkda1pz1lg45p-wayland-protocols-1.46/share/pkgconfig:/nix/store/5s110bxdr4jvxi3wjd2dwwjv9inn8ddp-libxkbcommon-1.11.0-dev/lib/pkgconfig:/nix/store/sf0xrxsq0xr2iq96b8awrwwliik5dm6r-libdrm-2.4.129-dev/lib/pkgconfig:/nix/store/pa3asawv8vx96lz2zsm89rrg13flr4wk-libinput-1.29.2-dev/lib/pkgconfig:/nix/store/qhw6n40d6mi5ply6rf4gmf6b0vvg1wai-systemd-minimal-libs-258.2-dev/lib/pkgconfig:/nix/store/qhw6n40d6mi5ply6rf4gmf6b0vvg1wai-systemd-minimal-libs-258.2-dev/share/pkgconfig:/nix/store/1cnvyk57f3wqlmbpidz0x2824ip9f8ly-pixman-0.46.4/lib/pkgconfig:/nix/store/8imqqzx7d62hfbn177lx18kqx08x8bma-seatd-0.9.1-dev/lib/pkgconfig:/nix/store/yydc0n1z5ag31x2vqv05n8iz498nvlm2-libglvnd-1.7.0-dev/lib/pkgconfig:/nix/store/ad2zpdj63hrnl90r01w9573m56gbkrh9-libffi-3.5.2-dev/lib/pkgconfig:/nix/store/nkgs0z67asdq920jxpkxazinzl11k0d3-libcap-2.77-dev/lib/pkgconfig:/nix/store/g3xkwyfnh4lifwyx6kmhnpz9bw2540hm-libxcb-1.17.0-dev/lib/pkgconfig:/nix/store/kg9a0vd3kq8p5hx40yw56z07v3vdlpif-xwayland-24.1.9/lib/pkgconfig:/nix/store/wd6lqkzpll686i5q9snd1c9c7glhcqqi-cairo-1.18.4-dev/lib/pkgconfig:/nix/store/pp2fbp19cabbdb2dhslf3qnxbsyyvnxm-fontconfig-2.17.1-dev/lib/pkgconfig:/nix/store/z7yvs4816g5xgmcj8122h0w97y5aa6l0-libxext-1.3.6-dev/lib/pkgconfig:/nix/store/fjp9qs2ww92pyag83fgq7amvqhph1dk7-libxau-1.0.12-dev/lib/pkgconfig:/nix/store/b4vxwmm7dr91x6ihh052k8ksxx5lybmj-libxrender-0.9.12-dev/lib/pkgconfig:/nix/store/j359ndglk11v5j47ra66lc1vxlk2dbjw-glib-2.86.3-dev/lib/pkgconfig:/nix/store/nd2qj4saqgbakhdnpl0f543lj9lalyfv-pango-1.57.0-dev/lib/pkgconfig:/nix/store/ayhcy9p6rshhda432gc1j3nqbhfklpi2-harfbuzz-12.2.0-dev/lib/pkgconfig:/nix/store/3salm1j8lhcyrkjblp7knj300raadc7g-graphite2-1.3.14-dev/lib/pkgconfig:/nix/store/21wibc25j73yapw3si0qbq1cyrv1dyfw-libxft-2.3.9-dev/lib/pkgconfig:/nix/store/bri9bpcyzlpcawwins7pinrsxrx69y6z-librsvg-2.61.3-dev/lib/pkgconfig:/nix/store/zsa9rgmanx1l9h7i76gfvv440n90vylg-gdk-pixbuf-2.44.4-dev/lib/pkgconfig:/nix/store/25y7kk3bbscnvvja6bixhz7wa95v2gvi-libtiff-4.7.1-dev/lib/pkgconfig:/nix/store/23xwha5lvfrgdhfqw8vm62as3p5dych2-vulkan-loader-1.4.335.0-dev/lib/pkgconfig:/nix/store/wgjgy1czmbdpq7i1p9kmcyhs5bn3p83c-glm-1.0.2/lib/pkgconfig'
export PKG_CONFIG_PATH
PS4='+ '
RANLIB='ranlib'
export RANLIB
READELF='readelf'
export READELF
SHELL='/nix/store/lw117lsr8d585xs63kx5k233impyrq7q-bash-5.3p3/bin/bash'
export SHELL
SIZE='size'
export SIZE
SOURCE_DATE_EPOCH='315532800'
export SOURCE_DATE_EPOCH
STRINGS='strings'
export STRINGS
STRIP='strip'
export STRIP
XDG_DATA_DIRS='/nix/store/wmb16s8hw1vd25vvidwnncpzq126lcz3-cmake-4.1.2/share:/nix/store/4vxdgiak2ab3s853wz18wj11rmzw8bv6-ninja-1.13.2/share:/nix/store/hxn2qrz1zmk5q01wsb7n3d58brzrsizb-pkg-config-wrapper-0.29.2/share:/nix/store/6kc4z3z8x47dhxzbha5dkywp5qwji1y7-wayland-scanner-1.24.0-dev/share:/nix/store/rky12ys8gxzglv2wqykbi1y8cb0dc080-wayland-scanner-1.24.0/share:/nix/store/lcd1j6avhn4gaacgbf4wkxx2yz7y3mx2-Whitesur-icon-theme-2025-08-02/share:/nix/store/wflnb8fxmq4lqx3ca91hlpv95vrqkv51-curl-8.17.0-dev/share:/nix/store/sk8x7aabysn1wr89i6a813ffii4x7i8w-brotli-1.2.0/share:/nix/store/3n3j314vsy174dnwks8dd6c0vrqvh8h6-krb5-1.22.1-lib/share:/nix/store/26f9s2k3cx2iri2rjh229na2p5xfmhmb-krb5-1.22.1/share:/nix/store/r7l6n43ifx8c37qg4wckdms8w6pdlka8-nghttp2-1.67.1/share:/nix/store/wpq7nwgwlic1yiwnsm5b3brkig3nn50z-libidn2-2.3.8-bin/share:/nix/store/kywwgk85nl83mpf10av3bvm2khdlq5ib-libidn2-2.3.8/share:/nix/store/dlkas2sgry8a2zrcs8xs4j2g1qa1jfj5-publicsuffix-list-0-unstable-2025-11-14/share:/nix/store/yrfd45bhkyv1hbsvl81987ri326qbb3z-libpsl-0.21.5/share:/nix/store/c2qsgf2832zi4n29gfkqgkjpvmbmxam6-zlib-1.3.1/share:/nix/store/w07dqm9na06xgf9vbs0xqrlzya9p7g6b-zstd-1.5.7/share:/nix/store/ss8rcralsb3b192371ma7kn9wvl3kdx4-freetype-2.13.3-dev/share:/nix/store/cvckjvajk2pdlx7i573fnd7ycrl46f17-freetype-2.13.3/share:/nix/store/a6q5zzy75h8k8wa7mnzqcpvnnr1wj8jy-lcms2-2.17-bin/share:/nix/store/vfvnfnq9xb69gzxqv1s5kbpldi99bzr2-xorgproto-2024.1/share:/nix/store/ii3ybky5dqjikcrw7vdnh1j76ssy0ycm-libx11-1.8.12/share:/nix/store/i6742f2vdh0y6k1qyc0m7srk6h7ks0nh-libwebp-1.6.0/share:/nix/store/dfj4g2rpz8yf1g6ybj7d37p4kmwsl772-imagemagick-7.1.2-11/share:/nix/store/n0cf9vad3va0js6606gcra8xhsnr0js7-gdb-16.3/share:/nix/store/n5m6p0j4anvf5cyy79nb2qn7smkzs176-gnumake-4.4.1/share:/nix/store/fryiyyhhilpqhx2wc4b6l5gnxc8w18rg-git-2.52.0/share:/nix/store/dk4dmm4bvskdjb84qp3yyg45mx3krysv-weston-14.0.1/share:/nix/store/3vs2fr2mazafcdwyza15bfhpmccx1k7z-patchelf-0.15.2/share'
export XDG_DATA_DIRS
__structuredAttrs=''
export __structuredAttrs
_substituteStream_has_warned_replace_deprecation='false'
buildInputs='/nix/store/vxm9x8hynlq20jsm38waw7ji3qprd6rq-wlroots-0.19.2 /nix/store/c3q7cp0nbvndl38znhqw3daihnirqj3g-wayland-1.24.0-dev /nix/store/zfl3y2dfih1qff84gldbkda1pz1lg45p-wayland-protocols-1.46 /nix/store/5s110bxdr4jvxi3wjd2dwwjv9inn8ddp-libxkbcommon-1.11.0-dev /nix/store/sf0xrxsq0xr2iq96b8awrwwliik5dm6r-libdrm-2.4.129-dev /nix/store/pa3asawv8vx96lz2zsm89rrg13flr4wk-libinput-1.29.2-dev /nix/store/1cnvyk57f3wqlmbpidz0x2824ip9f8ly-pixman-0.46.4 /nix/store/8imqqzx7d62hfbn177lx18kqx08x8bma-seatd-0.9.1-dev /nix/store/yydc0n1z5ag31x2vqv05n8iz498nvlm2-libglvnd-1.7.0-dev /nix/store/638y6yx9m2f5il7azqy1fn3hcczwjvfx-mesa-25.3.3 /nix/store/ad2zpdj63hrnl90r01w9573m56gbkrh9-libffi-3.5.2-dev /nix/store/nkgs0z67asdq920jxpkxazinzl11k0d3-libcap-2.77-dev /nix/store/qhw6n40d6mi5ply6rf4gmf6b0vvg1wai-systemd-minimal-libs-258.2-dev /nix/store/g3xkwyfnh4lifwyx6kmhnpz9bw2540hm-libxcb-1.17.0-dev /nix/store/kg9a0vd3kq8p5hx40yw56z07v3vdlpif-xwayland-24.1.9 /nix/store/wd6lqkzpll686i5q9snd1c9c7glhcqqi-cairo-1.18.4-dev /nix/store/nd2qj4saqgbakhdnpl0f543lj9lalyfv-pango-1.57.0-dev /nix/store/bri9bpcyzlpcawwins7pinrsxrx69y6z-librsvg-2.61.3-dev /nix/store/j359ndglk11v5j47ra66lc1vxlk2dbjw-glib-2.86.3-dev /nix/store/pp2fbp19cabbdb2dhslf3qnxbsyyvnxm-fontconfig-2.17.1-dev /nix/store/ss8rcralsb3b192371ma7kn9wvl3kdx4-freetype-2.13.3-dev /nix/store/3mxwvjpsqvaairm8b7j495znp6l04j51-vulkan-headers-1.4.335.0 /nix/store/23xwha5lvfrgdhfqw8vm62as3p5dych2-vulkan-loader-1.4.335.0-dev /nix/store/mc8i2kda3zpvfmz9mci7s1dsp3x5q448-vulkan-tools-1.4.335 /nix/store/94i9dxdbxpf6fyxsf7gviscnk5fs844x-vulkan-validation-layers-1.4.335.0 /nix/store/wgjgy1czmbdpq7i1p9kmcyhs5bn3p83c-glm-1.0.2'
export buildInputs
buildPhase='{ echo "------------------------------------------------------------";
  echo " WARNING: the existence of this path is not guaranteed.";
  echo " It is an internal implementation detail for pkgs.mkShell.";
  echo "------------------------------------------------------------";
  echo;
  # Record all build inputs as runtime dependencies
  export;
} >> "$out"
'
export buildPhase
builder='/nix/store/lw117lsr8d585xs63kx5k233impyrq7q-bash-5.3p3/bin/bash'
export builder
checkPhase='ninjaCheckPhase'
cmakeFlags=''
export cmakeFlags
configureFlags=''
export configureFlags
configurePhase='cmakeConfigurePhase'
defaultBuildInputs=''
defaultNativeBuildInputs='/nix/store/3vs2fr2mazafcdwyza15bfhpmccx1k7z-patchelf-0.15.2 /nix/store/9k7h0vvpnb8w4xcaypff4i76n1bsmwzz-update-autotools-gnu-config-scripts-hook /nix/store/0y5xmdb7qfvimjwbq7ibg1xdgkgjwqng-no-broken-symlinks.sh /nix/store/cv1d7p48379km6a85h4zp6kr86brh32q-audit-tmpdir.sh /nix/store/85clx3b0xkdf58jn161iy80y5223ilbi-compress-man-pages.sh /nix/store/p3l1a5y7nllfyrjn2krlwgcc3z0cd3fq-make-symlinks-relative.sh /nix/store/5yzw0vhkyszf2d179m0qfkgxmp5wjjx4-move-docs.sh /nix/store/fyaryjvghbkpfnsyw97hb3lyb37s1pd6-move-lib64.sh /nix/store/kd4xwxjpjxi71jkm6ka0np72if9rm3y0-move-sbin.sh /nix/store/pag6l61paj1dc9sv15l7bm5c17xn5kyk-move-systemd-user-units.sh /nix/store/cmzya9irvxzlkh7lfy6i82gbp0saxqj3-multiple-outputs.sh /nix/store/x8c40nfigps493a07sdr2pm5s9j1cdc0-patch-shebangs.sh /nix/store/cickvswrvann041nqxb0rxilc46svw1n-prune-libtool-files.sh /nix/store/xyff06pkhki3qy1ls77w10s0v79c9il0-reproducible-builds.sh /nix/store/z7k98578dfzi6l3hsvbivzm7hfqlk0zc-set-source-date-epoch-to-latest.sh /nix/store/pilsssjjdxvdphlg2h19p0bfx5q0jzkn-strip.sh /nix/store/r9wbjib6xxjkyb9yvjvrkl4sq61i2lyn-gcc-wrapper-15.2.0'
depsBuildBuild=''
export depsBuildBuild
depsBuildBuildPropagated=''
export depsBuildBuildPropagated
depsBuildTarget=''
export depsBuildTarget
depsBuildTargetPropagated=''
export depsBuildTargetPropagated
depsHostHost=''
export depsHostHost
depsHostHostPropagated=''
export depsHostHostPropagated
depsTargetTarget=''
export depsTargetTarget
depsTargetTargetPropagated=''
export depsTargetTargetPropagated
doCheck=''
export doCheck
doInstallCheck=''
export doInstallCheck
dontAddDisableDepTrack='1'
export dontAddDisableDepTrack
declare -a envBuildBuildHooks=()
declare -a envBuildHostHooks=()
declare -a envBuildTargetHooks=()
declare -a envHostHostHooks=('addCMakeParams' 'pkgConfigWrapper_addPkgConfigPath' 'ccWrapper_addCVars' 'bintoolsWrapper_addLDVars' 'ccWrapper_addCVars' 'ccWrapper_addCVars' 'bintoolsWrapper_addLDVars' 'gettextDataDirsHook' )
declare -a envHostTargetHooks=('addCMakeParams' 'pkgConfigWrapper_addPkgConfigPath' 'ccWrapper_addCVars' 'bintoolsWrapper_addLDVars' 'ccWrapper_addCVars' 'ccWrapper_addCVars' 'bintoolsWrapper_addLDVars' 'gettextDataDirsHook' )
declare -a envTargetTargetHooks=('make_glib_find_gsettings_schemas' 'findGdkPixbufLoaders' )
declare -a fixupOutputHooks=('if [ -z "${dontPatchELF-}" ]; then patchELF "$prefix"; fi' 'if [[ -z "${noAuditTmpdir-}" && -e "$prefix" ]]; then auditTmpdir "$prefix"; fi' 'if [ -z "${dontGzipMan-}" ]; then compressManPages "$prefix"; fi' '_moveLib64' '_moveSbin' '_moveSystemdUserUnits' 'patchShebangsAuto' '_pruneLibtoolFiles' '_doStrip' )
flag='-L/nix/store/wgjgy1czmbdpq7i1p9kmcyhs5bn3p83c-glm-1.0.2/lib'
guess='12'
iframework_seen=''
initialPath='/nix/store/d75200gb22v7p0703h5jrkgg8bqydk5q-coreutils-9.8 /nix/store/wd99g2j010fdkry0ws1bhxzm52w82ssx-findutils-4.10.0 /nix/store/20zvyjvxq9x2mkp7rbnvrwjjzq2n76hh-diffutils-3.12 /nix/store/k06ssckzrzn9jjvvs4n62m6567zmbx6x-gnused-4.9 /nix/store/qfmq7p42ak5yn389qvx7zpxkan5i4xiy-gnugrep-3.12 /nix/store/nmxm04dhkaqg1q6hai70n9zmzb0q49a5-gawk-5.3.2 /nix/store/k1lcfin159706lihwx5hhvl80dbij4jw-gnutar-1.35 /nix/store/0hv4z5s3r1h4lmvn0427mlxjxgvg34nr-gzip-1.14 /nix/store/p61ba9fdgx3358bpp18hv4rslf6n5bq6-bzip2-1.0.8-bin /nix/store/mkm3my2067305hdh7rzmi10npwr7y17f-gnumake-4.4.1 /nix/store/lw117lsr8d585xs63kx5k233impyrq7q-bash-5.3p3 /nix/store/clrf4mjwr8xcfpvway6w34wzvqc1hry4-patch-2.8 /nix/store/j1zc5jh0vi9sbxj09ldj4xklgm6kpf8n-xz-5.8.1-bin /nix/store/s2k48fw3y698j4kcvmw0520m06ihv2z4-file-5.45'
installPhase='ninjaInstallPhase'
isystem_seen=''
mesonFlags=''
export mesonFlags
name='opensef-dev-env'
export name
nativeBuildInputs='/nix/store/wmb16s8hw1vd25vvidwnncpzq126lcz3-cmake-4.1.2 /nix/store/4vxdgiak2ab3s853wz18wj11rmzw8bv6-ninja-1.13.2 /nix/store/hxn2qrz1zmk5q01wsb7n3d58brzrsizb-pkg-config-wrapper-0.29.2 /nix/store/6kc4z3z8x47dhxzbha5dkywp5qwji1y7-wayland-scanner-1.24.0-dev /nix/store/lcd1j6avhn4gaacgbf4wkxx2yz7y3mx2-Whitesur-icon-theme-2025-08-02 /nix/store/47y76bq8qcnpxizqgl0c0j15s7ciix5r-imagemagick-7.1.2-11-dev /nix/store/351bpjcf2l1n4vm06nwpq3cdhl6vbhx1-clang-wrapper-21.1.7 /nix/store/8ssdsraa07i1ciyl3az65xqnv44dfmx7-clang-wrapper-21.1.7 /nix/store/n0cf9vad3va0js6606gcra8xhsnr0js7-gdb-16.3 /nix/store/n5m6p0j4anvf5cyy79nb2qn7smkzs176-gnumake-4.4.1 /nix/store/fryiyyhhilpqhx2wc4b6l5gnxc8w18rg-git-2.52.0 /nix/store/dk4dmm4bvskdjb84qp3yyg45mx3krysv-weston-14.0.1'
export nativeBuildInputs
out='/mnt/c/Users/hp/Documents/VitusOS-Ares/outputs/out'
export out
outputBin='out'
outputDev='out'
outputDevdoc='REMOVE'
outputDevman='out'
outputDoc='out'
outputInclude='out'
outputInfo='out'
outputLib='out'
outputMan='out'
outputs='out'
export outputs
patches=''
export patches
phases='buildPhase'
export phases
pkg='/nix/store/r9wbjib6xxjkyb9yvjvrkl4sq61i2lyn-gcc-wrapper-15.2.0'
declare -a pkgsBuildBuild=()
declare -a pkgsBuildHost=('/nix/store/wmb16s8hw1vd25vvidwnncpzq126lcz3-cmake-4.1.2' '/nix/store/4vxdgiak2ab3s853wz18wj11rmzw8bv6-ninja-1.13.2' '/nix/store/hxn2qrz1zmk5q01wsb7n3d58brzrsizb-pkg-config-wrapper-0.29.2' '/nix/store/6kc4z3z8x47dhxzbha5dkywp5qwji1y7-wayland-scanner-1.24.0-dev' '/nix/store/ks51cz85rdrj4a374rfilgxf8b4c8d88-wayland-scanner-1.24.0-bin' '/nix/store/rky12ys8gxzglv2wqykbi1y8cb0dc080-wayland-scanner-1.24.0' '/nix/store/lcd1j6avhn4gaacgbf4wkxx2yz7y3mx2-Whitesur-icon-theme-2025-08-02' '/nix/store/47y76bq8qcnpxizqgl0c0j15s7ciix5r-imagemagick-7.1.2-11-dev' '/nix/store/wflnb8fxmq4lqx3ca91hlpv95vrqkv51-curl-8.17.0-dev' '/nix/store/aq62vlf3liwp8rpgn6wp0wsf3wbhcpqb-brotli-1.2.0-dev' '/nix/store/rj1cvkpc8nijyg7mv90c58s4kkhz3fiq-brotli-1.2.0-lib' '/nix/store/sk8x7aabysn1wr89i6a813ffii4x7i8w-brotli-1.2.0' '/nix/store/20dygnb42nwy7mbilwgql2lzz8h69pqz-krb5-1.22.1-dev' '/nix/store/3n3j314vsy174dnwks8dd6c0vrqvh8h6-krb5-1.22.1-lib' '/nix/store/26f9s2k3cx2iri2rjh229na2p5xfmhmb-krb5-1.22.1' '/nix/store/cxvk2hibqamfmgpbz9inbnvfyv49lr0h-nghttp2-1.67.1-dev' '/nix/store/bhhlhxd7xzvmgyvxhjls4x09xfxfnrwc-nghttp2-1.67.1-lib' '/nix/store/r7l6n43ifx8c37qg4wckdms8w6pdlka8-nghttp2-1.67.1' '/nix/store/bcqpmsh9w6xdy7yd81nkaij8g3vmd61g-nghttp3-1.13.1-dev' '/nix/store/qflgvhllyh98p52gh8wjlizfphhzsxb8-nghttp3-1.13.1' '/nix/store/ygwc32xg9dqylv1j3gh6rv2zlrhn4r7h-ngtcp2-1.18.0-dev' '/nix/store/m0xx7svz6967pw5knhqff8m0f48w0gj0-ngtcp2-1.18.0' '/nix/store/xy9v3ciqbqx2v9vqqn785hk22ndx55yd-libidn2-2.3.8-dev' '/nix/store/wpq7nwgwlic1yiwnsm5b3brkig3nn50z-libidn2-2.3.8-bin' '/nix/store/kywwgk85nl83mpf10av3bvm2khdlq5ib-libidn2-2.3.8' '/nix/store/2ivy0r8ab3bnps5957vfrxcjfcgad661-openssl-3.6.0-dev' '/nix/store/827b5xq1ghbbdrp1faa4fw32xj1m2p51-openssl-3.6.0-bin' '/nix/store/rfm5m2l26lqkskcvxn5bm5xqh6c8wqr5-openssl-3.6.0' '/nix/store/p59c986zyf31f923yhr8sqxdqnq3plnh-libpsl-0.21.5-dev' '/nix/store/dlkas2sgry8a2zrcs8xs4j2g1qa1jfj5-publicsuffix-list-0-unstable-2025-11-14' '/nix/store/yrfd45bhkyv1hbsvl81987ri326qbb3z-libpsl-0.21.5' '/nix/store/dp3sg21kr75b2059s94g3a2vg8dfajmv-libssh2-1.11.1-dev' '/nix/store/mxx1l7d0zhvqd9758rcbz0sbbaimkkq2-libssh2-1.11.1' '/nix/store/srnvjrr38j7xmpcmjpndd6mw31lsfjrl-zlib-1.3.1-dev' '/nix/store/c2qsgf2832zi4n29gfkqgkjpvmbmxam6-zlib-1.3.1' '/nix/store/di94mxmhhk390zzzv2gvavfwkgww7q6h-zstd-1.5.7-dev' '/nix/store/4i4q6abxjxnwf4j0ksc2zcb7byaxnrxi-zstd-1.5.7-bin' '/nix/store/w07dqm9na06xgf9vbs0xqrlzya9p7g6b-zstd-1.5.7' '/nix/store/cr9yz1bdfs9vj9cpr85ihrkgzgawgcwa-curl-8.17.0-bin' '/nix/store/0apq92vpf3f3kll8vl9nhjq1gaaffibk-curl-8.17.0' '/nix/store/c828rss7q5rra33phplrb12ayv2p9vgw-bzip2-1.0.8-dev' '/nix/store/p61ba9fdgx3358bpp18hv4rslf6n5bq6-bzip2-1.0.8-bin' '/nix/store/0c22zivvf0yspdvr2960rvmjgiwwi5wm-bzip2-1.0.8' '/nix/store/ss8rcralsb3b192371ma7kn9wvl3kdx4-freetype-2.13.3-dev' '/nix/store/li0sf0lhm30c83s1vi6filranamk9q3c-libpng-apng-1.6.52-dev' '/nix/store/94dcyz2ssdhr0s2srnafpj49ia6shsn1-libpng-apng-1.6.52' '/nix/store/cvckjvajk2pdlx7i573fnd7ycrl46f17-freetype-2.13.3' '/nix/store/5ljn72xffsivv2ya5ncrzkyf5nx94227-libjpeg-turbo-3.1.3-dev' '/nix/store/35bk8ab4h86q4ymq0irz8ivy3m8q33hf-libjpeg-turbo-3.1.3-bin' '/nix/store/alddrrr7703gs9lhw7lknzwr51kj4jsl-libjpeg-turbo-3.1.3' '/nix/store/lb33y3zdr3dzc948mf9v8n6sznc5b9bf-lcms2-2.17-dev' '/nix/store/a6q5zzy75h8k8wa7mnzqcpvnnr1wj8jy-lcms2-2.17-bin' '/nix/store/b8vsara78lvpyqz25n03qljh0587fqv5-lcms2-2.17' '/nix/store/vx5qrqwr1600p7db7ahykcgqbakk7gzc-libx11-1.8.12-dev' '/nix/store/vfvnfnq9xb69gzxqv1s5kbpldi99bzr2-xorgproto-2024.1' '/nix/store/ii3ybky5dqjikcrw7vdnh1j76ssy0ycm-libx11-1.8.12' '/nix/store/s75hddpdhzzakb6jbzacq2q5bgh2y36k-libxt-1.3.1-dev' '/nix/store/3xf66ysnxf1d5i37fnl144781iyva4q3-libsm-1.2.6-dev' '/nix/store/5blg2pq37crjblq81sklbr7imya83wnb-libice-1.1.2-dev' '/nix/store/7yr1yymvbrkqyqxfp6hf0y7k2f8s91c7-libice-1.1.2' '/nix/store/s3syj0fs1a4rfi51fvgjmngagfyiiz9i-libsm-1.2.6' '/nix/store/5lm05s676bd9r6kgqnymbghzp6lg9xyw-libxt-1.3.1' '/nix/store/i6742f2vdh0y6k1qyc0m7srk6h7ks0nh-libwebp-1.6.0' '/nix/store/vlcfhfmsm11490mp7fd2n0g9rnapgpid-fftw-double-3.3.10-dev' '/nix/store/qpg5bb13x7p13d02kb8rv1f4333glzlw-fftw-double-3.3.10' '/nix/store/dfj4g2rpz8yf1g6ybj7d37p4kmwsl772-imagemagick-7.1.2-11' '/nix/store/351bpjcf2l1n4vm06nwpq3cdhl6vbhx1-clang-wrapper-21.1.7' '/nix/store/n21wcnpiydy3fd6kqxkwnzvwvaibmg11-binutils-wrapper-2.44' '/nix/store/8ssdsraa07i1ciyl3az65xqnv44dfmx7-clang-wrapper-21.1.7' '/nix/store/n0cf9vad3va0js6606gcra8xhsnr0js7-gdb-16.3' '/nix/store/n5m6p0j4anvf5cyy79nb2qn7smkzs176-gnumake-4.4.1' '/nix/store/fryiyyhhilpqhx2wc4b6l5gnxc8w18rg-git-2.52.0' '/nix/store/dk4dmm4bvskdjb84qp3yyg45mx3krysv-weston-14.0.1' '/nix/store/3vs2fr2mazafcdwyza15bfhpmccx1k7z-patchelf-0.15.2' '/nix/store/9k7h0vvpnb8w4xcaypff4i76n1bsmwzz-update-autotools-gnu-config-scripts-hook' '/nix/store/0y5xmdb7qfvimjwbq7ibg1xdgkgjwqng-no-broken-symlinks.sh' '/nix/store/cv1d7p48379km6a85h4zp6kr86brh32q-audit-tmpdir.sh' '/nix/store/85clx3b0xkdf58jn161iy80y5223ilbi-compress-man-pages.sh' '/nix/store/p3l1a5y7nllfyrjn2krlwgcc3z0cd3fq-make-symlinks-relative.sh' '/nix/store/5yzw0vhkyszf2d179m0qfkgxmp5wjjx4-move-docs.sh' '/nix/store/fyaryjvghbkpfnsyw97hb3lyb37s1pd6-move-lib64.sh' '/nix/store/kd4xwxjpjxi71jkm6ka0np72if9rm3y0-move-sbin.sh' '/nix/store/pag6l61paj1dc9sv15l7bm5c17xn5kyk-move-systemd-user-units.sh' '/nix/store/cmzya9irvxzlkh7lfy6i82gbp0saxqj3-multiple-outputs.sh' '/nix/store/x8c40nfigps493a07sdr2pm5s9j1cdc0-patch-shebangs.sh' '/nix/store/cickvswrvann041nqxb0rxilc46svw1n-prune-libtool-files.sh' '/nix/store/xyff06pkhki3qy1ls77w10s0v79c9il0-reproducible-builds.sh' '/nix/store/z7k98578dfzi6l3hsvbivzm7hfqlk0zc-set-source-date-epoch-to-latest.sh' '/nix/store/pilsssjjdxvdphlg2h19p0bfx5q0jzkn-strip.sh' '/nix/store/r9wbjib6xxjkyb9yvjvrkl4sq61i2lyn-gcc-wrapper-15.2.0' '/nix/store/dwiyp91lmxq864plaas14jm14m87sg3f-binutils-wrapper-2.44' )
declare -a pkgsBuildTarget=()
declare -a pkgsHostHost=('/nix/store/w7rpcgzwhqdcrsxzj3ml4njjwv0bar82-compiler-rt-libc-21.1.7-dev' '/nix/store/3mdkalklfdwfdwdi19x1zzlzl017y5xl-compiler-rt-libc-21.1.7' '/nix/store/gwyq2ns8nzgf6q3cvr1k8gasfn03khcp-libcxx-21.1.7-dev' '/nix/store/9d1kansnki5ipsfk9gv1ajb6fxd218ik-libcxx-21.1.7' )
declare -a pkgsHostTarget=('/nix/store/vxm9x8hynlq20jsm38waw7ji3qprd6rq-wlroots-0.19.2' '/nix/store/c3q7cp0nbvndl38znhqw3daihnirqj3g-wayland-1.24.0-dev' '/nix/store/7m6x6b4ljrkmkbhvbxdcacpraxj49xkj-wayland-1.24.0' '/nix/store/zfl3y2dfih1qff84gldbkda1pz1lg45p-wayland-protocols-1.46' '/nix/store/5s110bxdr4jvxi3wjd2dwwjv9inn8ddp-libxkbcommon-1.11.0-dev' '/nix/store/91mg839gbvgz1svzlh1jvz0pqvx7ylwh-libxkbcommon-1.11.0' '/nix/store/sf0xrxsq0xr2iq96b8awrwwliik5dm6r-libdrm-2.4.129-dev' '/nix/store/0j7wz5lhxzzjnq637xyjv9arq6irplks-libdrm-2.4.129-bin' '/nix/store/sixic8a4adxz72swww4fcls0h5sabjdf-libdrm-2.4.129' '/nix/store/pa3asawv8vx96lz2zsm89rrg13flr4wk-libinput-1.29.2-dev' '/nix/store/qhw6n40d6mi5ply6rf4gmf6b0vvg1wai-systemd-minimal-libs-258.2-dev' '/nix/store/n234iswy40dzhd7aw6ak0wfrbyjpvbw4-systemd-minimal-libs-258.2' '/nix/store/248zkd6222jr7xv5jlwrdj70c15g3qvr-libinput-1.29.2-bin' '/nix/store/pfaavvrfb1lhx26zwllc4vm5jyzfl1hh-libinput-1.29.2' '/nix/store/1cnvyk57f3wqlmbpidz0x2824ip9f8ly-pixman-0.46.4' '/nix/store/8imqqzx7d62hfbn177lx18kqx08x8bma-seatd-0.9.1-dev' '/nix/store/dyqhp6gciwzm31k2pc8g92rylqk4rfsz-seatd-0.9.1-bin' '/nix/store/zjqacif3aa0idg3k18dlsz37iyzn0fqm-seatd-0.9.1' '/nix/store/yydc0n1z5ag31x2vqv05n8iz498nvlm2-libglvnd-1.7.0-dev' '/nix/store/x4sc437gdbhrfbm4b0hja9lfv38njvi1-libglvnd-1.7.0' '/nix/store/638y6yx9m2f5il7azqy1fn3hcczwjvfx-mesa-25.3.3' '/nix/store/ad2zpdj63hrnl90r01w9573m56gbkrh9-libffi-3.5.2-dev' '/nix/store/1z031fwbsc8jhmm7i39r6z6m1xn7rbza-libffi-3.5.2' '/nix/store/nkgs0z67asdq920jxpkxazinzl11k0d3-libcap-2.77-dev' '/nix/store/n16jmmckg4fpqk97542hbdvlryxray79-libcap-2.77-lib' '/nix/store/cm1jl79is5qb7zmqg8b8nmrdd7nqdjpk-libcap-2.77' '/nix/store/g3xkwyfnh4lifwyx6kmhnpz9bw2540hm-libxcb-1.17.0-dev' '/nix/store/m0jzvlr5gl7zvdn0fr7c09mjwx65fx58-libxcb-1.17.0' '/nix/store/kg9a0vd3kq8p5hx40yw56z07v3vdlpif-xwayland-24.1.9' '/nix/store/wd6lqkzpll686i5q9snd1c9c7glhcqqi-cairo-1.18.4-dev' '/nix/store/pp2fbp19cabbdb2dhslf3qnxbsyyvnxm-fontconfig-2.17.1-dev' '/nix/store/ss8rcralsb3b192371ma7kn9wvl3kdx4-freetype-2.13.3-dev' '/nix/store/srnvjrr38j7xmpcmjpndd6mw31lsfjrl-zlib-1.3.1-dev' '/nix/store/c2qsgf2832zi4n29gfkqgkjpvmbmxam6-zlib-1.3.1' '/nix/store/c828rss7q5rra33phplrb12ayv2p9vgw-bzip2-1.0.8-dev' '/nix/store/p61ba9fdgx3358bpp18hv4rslf6n5bq6-bzip2-1.0.8-bin' '/nix/store/0c22zivvf0yspdvr2960rvmjgiwwi5wm-bzip2-1.0.8' '/nix/store/aq62vlf3liwp8rpgn6wp0wsf3wbhcpqb-brotli-1.2.0-dev' '/nix/store/rj1cvkpc8nijyg7mv90c58s4kkhz3fiq-brotli-1.2.0-lib' '/nix/store/sk8x7aabysn1wr89i6a813ffii4x7i8w-brotli-1.2.0' '/nix/store/li0sf0lhm30c83s1vi6filranamk9q3c-libpng-apng-1.6.52-dev' '/nix/store/94dcyz2ssdhr0s2srnafpj49ia6shsn1-libpng-apng-1.6.52' '/nix/store/cvckjvajk2pdlx7i573fnd7ycrl46f17-freetype-2.13.3' '/nix/store/5pzrv0bbcpw6pq4wi6vhjjv597ki2346-fontconfig-2.17.1-bin' '/nix/store/va64srrrxljdrkh4w7y97gl3rzj6dgj9-fontconfig-2.17.1-lib' '/nix/store/z7yvs4816g5xgmcj8122h0w97y5aa6l0-libxext-1.3.6-dev' '/nix/store/vfvnfnq9xb69gzxqv1s5kbpldi99bzr2-xorgproto-2024.1' '/nix/store/fjp9qs2ww92pyag83fgq7amvqhph1dk7-libxau-1.0.12-dev' '/nix/store/whgxask7xm93jimlqrc2gm3j46p038gi-libxau-1.0.12' '/nix/store/zshby6nalhw4mvap0rr97hv042808c2k-libxext-1.3.6' '/nix/store/b4vxwmm7dr91x6ihh052k8ksxx5lybmj-libxrender-0.9.12-dev' '/nix/store/vx5qrqwr1600p7db7ahykcgqbakk7gzc-libx11-1.8.12-dev' '/nix/store/ii3ybky5dqjikcrw7vdnh1j76ssy0ycm-libx11-1.8.12' '/nix/store/8xmc6wk01sgy6djqchvwf168vk6sp0qa-libxrender-0.9.12' '/nix/store/j359ndglk11v5j47ra66lc1vxlk2dbjw-glib-2.86.3-dev' '/nix/store/1dax567dy7cf55x25a1ryg6z6hl3lvzw-gettext-0.25.1' '/nix/store/yxi1pmrgn8vsrrncz6pdlxhr5lsc3ysr-glibc-iconv-2.40' '/nix/store/4c9rdn3nsq3lv44zqsbb865fm174sxx9-glib-2.86.3-bin' '/nix/store/f7rcazhd826xlcz43il4vafv28888cgj-glib-2.86.3' '/nix/store/733dh2bc55l8khcnkanmz1f99m77xjm0-cairo-1.18.4' '/nix/store/nd2qj4saqgbakhdnpl0f543lj9lalyfv-pango-1.57.0-dev' '/nix/store/ayhcy9p6rshhda432gc1j3nqbhfklpi2-harfbuzz-12.2.0-dev' '/nix/store/3salm1j8lhcyrkjblp7knj300raadc7g-graphite2-1.3.14-dev' '/nix/store/7n6bbv5ll4wpjq1kfbch7b16s74my77y-graphite2-1.3.14' '/nix/store/40b7x2n2yylv79rgnavw3xk7yp9308jx-harfbuzz-12.2.0' '/nix/store/21wibc25j73yapw3si0qbq1cyrv1dyfw-libxft-2.3.9-dev' '/nix/store/wbnn8hmnfcd7pmk0n18fr7azsqrml1d0-libxft-2.3.9' '/nix/store/qn4xlakafmwk72x5dj4bhlgssxim4xf0-pango-1.57.0-bin' '/nix/store/7qlq78jh6n35v1xpxyd6c279nbf0r88f-pango-1.57.0' '/nix/store/bri9bpcyzlpcawwins7pinrsxrx69y6z-librsvg-2.61.3-dev' '/nix/store/zsa9rgmanx1l9h7i76gfvv440n90vylg-gdk-pixbuf-2.44.4-dev' '/nix/store/25y7kk3bbscnvvja6bixhz7wa95v2gvi-libtiff-4.7.1-dev' '/nix/store/7wg39dxpf0b4vigv4aib7hzqf6134la1-libtiff-4.7.1-bin' '/nix/store/n1276dzpdi463s5nb2fsmysnxkirn3j6-libtiff-4.7.1' '/nix/store/5ljn72xffsivv2ya5ncrzkyf5nx94227-libjpeg-turbo-3.1.3-dev' '/nix/store/35bk8ab4h86q4ymq0irz8ivy3m8q33hf-libjpeg-turbo-3.1.3-bin' '/nix/store/alddrrr7703gs9lhw7lknzwr51kj4jsl-libjpeg-turbo-3.1.3' '/nix/store/5k9nfx38ckb0wkwsgy925ddzdhrvj16s-gdk-pixbuf-2.44.4' '/nix/store/8gz43a07r76vahgqlx88iz9yi22shdv0-librsvg-2.61.3' '/nix/store/3mxwvjpsqvaairm8b7j495znp6l04j51-vulkan-headers-1.4.335.0' '/nix/store/23xwha5lvfrgdhfqw8vm62as3p5dych2-vulkan-loader-1.4.335.0-dev' '/nix/store/jzng5i2xwsdlf3h60a5s6qzr2808c9zc-vulkan-loader-1.4.335.0' '/nix/store/mc8i2kda3zpvfmz9mci7s1dsp3x5q448-vulkan-tools-1.4.335' '/nix/store/94i9dxdbxpf6fyxsf7gviscnk5fs844x-vulkan-validation-layers-1.4.335.0' '/nix/store/wgjgy1czmbdpq7i1p9kmcyhs5bn3p83c-glm-1.0.2' )
declare -a pkgsTargetTarget=()
declare -a postFixupHooks=('cmakePcfileCheckPhase' 'noBrokenSymlinksInAllOutputs' '_makeSymlinksRelative' '_multioutPropagateDev' )
declare -a postHooks=('makeCmakeFindLibs' )
declare -a postInstallHooks=('glibPostInstallHook' )
declare -a postUnpackHooks=('_updateSourceDateEpochFromSourceRoot' )
declare -a preConfigureHooks=('_multioutConfig' )
preConfigurePhases=' updateAutotoolsGnuConfigScriptsPhase'
declare -a preFixupHooks=('_moveToShare' '_multioutDocs' '_multioutDevs' )
preInstallPhases=' glibPreInstallPhase'
preferLocalBuild='1'
export preferLocalBuild
prefix='/mnt/c/Users/hp/Documents/VitusOS-Ares/outputs/out'
declare -a propagatedBuildDepFiles=('propagated-build-build-deps' 'propagated-native-build-inputs' 'propagated-build-target-deps' )
propagatedBuildInputs=''
export propagatedBuildInputs
declare -a propagatedHostDepFiles=('propagated-host-host-deps' 'propagated-build-inputs' )
propagatedNativeBuildInputs=''
export propagatedNativeBuildInputs
declare -a propagatedTargetDepFiles=('propagated-target-target-deps' )
role_post=''
setOutputFlags=''
shell='/nix/store/lw117lsr8d585xs63kx5k233impyrq7q-bash-5.3p3/bin/bash'
export shell
shellHook='echo "╔════════════════════════════════════════════╗"
echo "║     VitusOS Ares Dev Shell                 ║"
echo "╚════════════════════════════════════════════╝"

# Fix PKG_CONFIG_PATH explicitly for split outputs (unstable channel quirks)
export PKG_CONFIG_PATH="/nix/store/vxm9x8hynlq20jsm38waw7ji3qprd6rq-wlroots-0.19.2/lib/pkgconfig:/nix/store/c3q7cp0nbvndl38znhqw3daihnirqj3g-wayland-1.24.0-dev/lib/pkgconfig:/nix/store/zfl3y2dfih1qff84gldbkda1pz1lg45p-wayland-protocols-1.46/share/pkgconfig:/nix/store/wd6lqkzpll686i5q9snd1c9c7glhcqqi-cairo-1.18.4-dev/lib/pkgconfig:/nix/store/nd2qj4saqgbakhdnpl0f543lj9lalyfv-pango-1.57.0-dev/lib/pkgconfig:/nix/store/5s110bxdr4jvxi3wjd2dwwjv9inn8ddp-libxkbcommon-1.11.0-dev/lib/pkgconfig:/nix/store/638y6yx9m2f5il7azqy1fn3hcczwjvfx-mesa-25.3.3/lib/pkgconfig:$PKG_CONFIG_PATH"

echo "Detecting wlroots version..."
if pkg-config --exists wlroots-0.19; then
    echo "✅ Found wlroots-0.19"
elif pkg-config --exists wlroots; then
    echo "✅ Found wlroots (generic)"
else
    echo "⚠️  Note: '\''wlroots'\'' alias not found, but '\''wlroots-0.19'\'' likely exists."
    echo "    CMake has been updated to find 0.19 automatically."
fi

export CC=clang
export CXX=clang++
echo "Using Clang compiler: $(clang --version | head -n1)"
'
export shellHook
stdenv='/nix/store/n1k7lm072r5k3g6v6wb91d2q4sxcxddm-stdenv-linux'
export stdenv
strictDeps=''
export strictDeps
system='x86_64-linux'
export system
declare -a unpackCmdHooks=('_defaultUnpack' )
_activatePkgs ()
{
 
    local hostOffset targetOffset;
    local pkg;
    for hostOffset in "${allPlatOffsets[@]}";
    do
        local pkgsVar="${pkgAccumVarVars[hostOffset + 1]}";
        for targetOffset in "${allPlatOffsets[@]}";
        do
            (( hostOffset <= targetOffset )) || continue;
            local pkgsRef="${pkgsVar}[$targetOffset - $hostOffset]";
            local pkgsSlice="${!pkgsRef}[@]";
            for pkg in ${!pkgsSlice+"${!pkgsSlice}"};
            do
                activatePackage "$pkg" "$hostOffset" "$targetOffset";
            done;
        done;
    done
}
_addRpathPrefix ()
{
 
    if [ "${NIX_NO_SELF_RPATH:-0}" != 1 ]; then
        export NIX_LDFLAGS="-rpath $1/lib ${NIX_LDFLAGS-}";
    fi
}
_addToEnv ()
{
 
    local depHostOffset depTargetOffset;
    local pkg;
    for depHostOffset in "${allPlatOffsets[@]}";
    do
        local hookVar="${pkgHookVarVars[depHostOffset + 1]}";
        local pkgsVar="${pkgAccumVarVars[depHostOffset + 1]}";
        for depTargetOffset in "${allPlatOffsets[@]}";
        do
            (( depHostOffset <= depTargetOffset )) || continue;
            local hookRef="${hookVar}[$depTargetOffset - $depHostOffset]";
            if [[ -z "${strictDeps-}" ]]; then
                local visitedPkgs="";
                for pkg in "${pkgsBuildBuild[@]}" "${pkgsBuildHost[@]}" "${pkgsBuildTarget[@]}" "${pkgsHostHost[@]}" "${pkgsHostTarget[@]}" "${pkgsTargetTarget[@]}";
                do
                    if [[ "$visitedPkgs" = *"$pkg"* ]]; then
                        continue;
                    fi;
                    runHook "${!hookRef}" "$pkg";
                    visitedPkgs+=" $pkg";
                done;
            else
                local pkgsRef="${pkgsVar}[$depTargetOffset - $depHostOffset]";
                local pkgsSlice="${!pkgsRef}[@]";
                for pkg in ${!pkgsSlice+"${!pkgsSlice}"};
                do
                    runHook "${!hookRef}" "$pkg";
                done;
            fi;
        done;
    done
}
_allFlags ()
{
 
    export system pname name version;
    while IFS='' read -r varName; do
        nixTalkativeLog "@${varName}@ -> ${!varName}";
        args+=("--subst-var" "$varName");
    done < <(awk 'BEGIN { for (v in ENVIRON) if (v ~ /^[a-z][a-zA-Z0-9_]*$/) print v }')
}
_assignFirst ()
{
 
    local varName="$1";
    local _var;
    local REMOVE=REMOVE;
    shift;
    for _var in "$@";
    do
        if [ -n "${!_var-}" ]; then
            eval "${varName}"="${_var}";
            return;
        fi;
    done;
    echo;
    echo "error: _assignFirst: could not find a non-empty variable whose name to assign to ${varName}.";
    echo "       The following variables were all unset or empty:";
    echo "           $*";
    if [ -z "${out:-}" ]; then
        echo '       If you do not want an "out" output in your derivation, make sure to define';
        echo '       the other specific required outputs. This can be achieved by picking one';
        echo "       of the above as an output.";
        echo '       You do not have to remove "out" if you want to have a different default';
        echo '       output, because the first output is taken as a default.';
        echo;
    fi;
    return 1
}
_callImplicitHook ()
{
 
    local def="$1";
    local hookName="$2";
    if declare -F "$hookName" > /dev/null; then
        nixTalkativeLog "calling implicit '$hookName' function hook";
        "$hookName";
    else
        if type -p "$hookName" > /dev/null; then
            nixTalkativeLog "sourcing implicit '$hookName' script hook";
            source "$hookName";
        else
            if [ -n "${!hookName:-}" ]; then
                nixTalkativeLog "evaling implicit '$hookName' string hook";
                eval "${!hookName}";
            else
                return "$def";
            fi;
        fi;
    fi
}
_defaultUnpack ()
{
 
    local fn="$1";
    local destination;
    if [ -d "$fn" ]; then
        destination="$(stripHash "$fn")";
        if [ -e "$destination" ]; then
            echo "Cannot copy $fn to $destination: destination already exists!";
            echo "Did you specify two \"srcs\" with the same \"name\"?";
            return 1;
        fi;
        cp -r --preserve=timestamps --reflink=auto -- "$fn" "$destination";
    else
        case "$fn" in 
            *.tar.xz | *.tar.lzma | *.txz)
                ( XZ_OPT="--threads=$NIX_BUILD_CORES" xz -d < "$fn";
                true ) | tar xf - --mode=+w --warning=no-timestamp
            ;;
            *.tar | *.tar.* | *.tgz | *.tbz2 | *.tbz)
                tar xf "$fn" --mode=+w --warning=no-timestamp
            ;;
            *)
                return 1
            ;;
        esac;
    fi
}
_doStrip ()
{
 
    local -ra flags=(dontStripHost dontStripTarget);
    local -ra debugDirs=(stripDebugList stripDebugListTarget);
    local -ra allDirs=(stripAllList stripAllListTarget);
    local -ra stripCmds=(STRIP STRIP_FOR_TARGET);
    local -ra ranlibCmds=(RANLIB RANLIB_FOR_TARGET);
    stripDebugList=${stripDebugList[*]:-lib lib32 lib64 libexec bin sbin Applications Library/Frameworks};
    stripDebugListTarget=${stripDebugListTarget[*]:-};
    stripAllList=${stripAllList[*]:-};
    stripAllListTarget=${stripAllListTarget[*]:-};
    local i;
    for i in ${!stripCmds[@]};
    do
        local -n flag="${flags[$i]}";
        local -n debugDirList="${debugDirs[$i]}";
        local -n allDirList="${allDirs[$i]}";
        local -n stripCmd="${stripCmds[$i]}";
        local -n ranlibCmd="${ranlibCmds[$i]}";
        if [[ -n "${dontStrip-}" || -n "${flag-}" ]] || ! type -f "${stripCmd-}" 2> /dev/null 1>&2; then
            continue;
        fi;
        stripDirs "$stripCmd" "$ranlibCmd" "$debugDirList" "${stripDebugFlags[*]:--S -p}";
        stripDirs "$stripCmd" "$ranlibCmd" "$allDirList" "${stripAllFlags[*]:--s -p}";
    done
}
_eval ()
{
 
    if declare -F "$1" > /dev/null 2>&1; then
        "$@";
    else
        eval "$1";
    fi
}
_logHook ()
{
 
    if [[ -z ${NIX_LOG_FD-} ]]; then
        return;
    fi;
    local hookKind="$1";
    local hookExpr="$2";
    shift 2;
    if declare -F "$hookExpr" > /dev/null 2>&1; then
        nixTalkativeLog "calling '$hookKind' function hook '$hookExpr'" "$@";
    else
        if type -p "$hookExpr" > /dev/null; then
            nixTalkativeLog "sourcing '$hookKind' script hook '$hookExpr'";
        else
            if [[ "$hookExpr" != "_callImplicitHook"* ]]; then
                local exprToOutput;
                if [[ ${NIX_DEBUG:-0} -ge 5 ]]; then
                    exprToOutput="$hookExpr";
                else
                    local hookExprLine;
                    while IFS= read -r hookExprLine; do
                        hookExprLine="${hookExprLine#"${hookExprLine%%[![:space:]]*}"}";
                        if [[ -n "$hookExprLine" ]]; then
                            exprToOutput+="$hookExprLine\\n ";
                        fi;
                    done <<< "$hookExpr";
                    exprToOutput="${exprToOutput%%\\n }";
                fi;
                nixTalkativeLog "evaling '$hookKind' string hook '$exprToOutput'";
            fi;
        fi;
    fi
}
_makeSymlinksRelative ()
{
 
    local prefixes;
    prefixes=();
    for output in $(getAllOutputNames);
    do
        [ ! -e "${!output}" ] && continue;
        prefixes+=("${!output}");
    done;
    find "${prefixes[@]}" -type l -printf '%H\0%p\0' | xargs -0 -n2 -r -P "$NIX_BUILD_CORES" sh -c '
      output="$1"
      link="$2"

      linkTarget=$(readlink "$link")

      # only touch links that point inside the same output tree
      [[ $linkTarget == "$output"/* ]] || exit 0

      if [ ! -e "$linkTarget" ]; then
        echo "the symlink $link is broken, it points to $linkTarget (which is missing)"
      fi

      echo "making symlink relative: $link"
      ln -snrf "$linkTarget" "$link"
    ' _
}
_moveLib64 ()
{
 
    if [ "${dontMoveLib64-}" = 1 ]; then
        return;
    fi;
    if [ ! -e "$prefix/lib64" -o -L "$prefix/lib64" ]; then
        return;
    fi;
    echo "moving $prefix/lib64/* to $prefix/lib";
    mkdir -p $prefix/lib;
    shopt -s dotglob;
    for i in $prefix/lib64/*;
    do
        mv --no-clobber "$i" $prefix/lib;
    done;
    shopt -u dotglob;
    rmdir $prefix/lib64;
    ln -s lib $prefix/lib64
}
_moveSbin ()
{
 
    if [ "${dontMoveSbin-}" = 1 ]; then
        return;
    fi;
    if [ ! -e "$prefix/sbin" -o -L "$prefix/sbin" ]; then
        return;
    fi;
    echo "moving $prefix/sbin/* to $prefix/bin";
    mkdir -p $prefix/bin;
    shopt -s dotglob;
    for i in $prefix/sbin/*;
    do
        mv "$i" $prefix/bin;
    done;
    shopt -u dotglob;
    rmdir $prefix/sbin;
    ln -s bin $prefix/sbin
}
_moveSystemdUserUnits ()
{
 
    if [ "${dontMoveSystemdUserUnits:-0}" = 1 ]; then
        return;
    fi;
    if [ ! -e "${prefix:?}/lib/systemd/user" ]; then
        return;
    fi;
    local source="$prefix/lib/systemd/user";
    local target="$prefix/share/systemd/user";
    echo "moving $source/* to $target";
    mkdir -p "$target";
    ( shopt -s dotglob;
    for i in "$source"/*;
    do
        mv "$i" "$target";
    done );
    rmdir "$source";
    ln -s "$target" "$source"
}
_moveToShare ()
{
 
    if [ -n "$__structuredAttrs" ]; then
        if [ -z "${forceShare-}" ]; then
            forceShare=(man doc info);
        fi;
    else
        forceShare=(${forceShare:-man doc info});
    fi;
    if [[ -z "$out" ]]; then
        return;
    fi;
    for d in "${forceShare[@]}";
    do
        if [ -d "$out/$d" ]; then
            if [ -d "$out/share/$d" ]; then
                echo "both $d/ and share/$d/ exist!";
            else
                echo "moving $out/$d to $out/share/$d";
                mkdir -p $out/share;
                mv $out/$d $out/share/;
            fi;
        fi;
    done
}
_multioutConfig ()
{
 
    if [ "$(getAllOutputNames)" = "out" ] || [ -z "${setOutputFlags-1}" ]; then
        return;
    fi;
    if [ -z "${shareDocName:-}" ]; then
        local confScript="${configureScript:-}";
        if [ -z "$confScript" ] && [ -x ./configure ]; then
            confScript=./configure;
        fi;
        if [ -f "$confScript" ]; then
            local shareDocName="$(sed -n "s/^PACKAGE_TARNAME='\(.*\)'$/\1/p" < "$confScript")";
        fi;
        if [ -z "$shareDocName" ] || echo "$shareDocName" | grep -q '[^a-zA-Z0-9_-]'; then
            shareDocName="$(echo "$name" | sed 's/-[^a-zA-Z].*//')";
        fi;
    fi;
    prependToVar configureFlags --bindir="${!outputBin}"/bin --sbindir="${!outputBin}"/sbin --includedir="${!outputInclude}"/include --mandir="${!outputMan}"/share/man --infodir="${!outputInfo}"/share/info --docdir="${!outputDoc}"/share/doc/"${shareDocName}" --libdir="${!outputLib}"/lib --libexecdir="${!outputLib}"/libexec --localedir="${!outputLib}"/share/locale;
    prependToVar installFlags pkgconfigdir="${!outputDev}"/lib/pkgconfig m4datadir="${!outputDev}"/share/aclocal aclocaldir="${!outputDev}"/share/aclocal
}
_multioutDevs ()
{
 
    if [ "$(getAllOutputNames)" = "out" ] || [ -z "${moveToDev-1}" ]; then
        return;
    fi;
    moveToOutput include "${!outputInclude}";
    moveToOutput lib/pkgconfig "${!outputDev}";
    moveToOutput share/pkgconfig "${!outputDev}";
    moveToOutput lib/cmake "${!outputDev}";
    moveToOutput share/aclocal "${!outputDev}";
    for f in "${!outputDev}"/{lib,share}/pkgconfig/*.pc;
    do
        echo "Patching '$f' includedir to output ${!outputInclude}";
        sed -i "/^includedir=/s,=\${prefix},=${!outputInclude}," "$f";
    done
}
_multioutDocs ()
{
 
    local REMOVE=REMOVE;
    moveToOutput share/info "${!outputInfo}";
    moveToOutput share/doc "${!outputDoc}";
    moveToOutput share/gtk-doc "${!outputDevdoc}";
    moveToOutput share/devhelp/books "${!outputDevdoc}";
    moveToOutput share/man "${!outputMan}";
    moveToOutput share/man/man3 "${!outputDevman}"
}
_multioutPropagateDev ()
{
 
    if [ "$(getAllOutputNames)" = "out" ]; then
        return;
    fi;
    local outputFirst;
    for outputFirst in $(getAllOutputNames);
    do
        break;
    done;
    local propagaterOutput="$outputDev";
    if [ -z "$propagaterOutput" ]; then
        propagaterOutput="$outputFirst";
    fi;
    if [ -z "${propagatedBuildOutputs+1}" ]; then
        local po_dirty="$outputBin $outputInclude $outputLib";
        set +o pipefail;
        propagatedBuildOutputs=`echo "$po_dirty"             | tr -s ' ' '\n' | grep -v -F "$propagaterOutput"             | sort -u | tr '\n' ' ' `;
        set -o pipefail;
    fi;
    if [ -z "$propagatedBuildOutputs" ]; then
        return;
    fi;
    mkdir -p "${!propagaterOutput}"/nix-support;
    for output in $propagatedBuildOutputs;
    do
        echo -n " ${!output}" >> "${!propagaterOutput}"/nix-support/propagated-build-inputs;
    done
}
_nixLogWithLevel ()
{
 
    [[ -z ${NIX_LOG_FD-} || ${NIX_DEBUG:-0} -lt ${1:?} ]] && return 0;
    local logLevel;
    case "${1:?}" in 
        0)
            logLevel=ERROR
        ;;
        1)
            logLevel=WARN
        ;;
        2)
            logLevel=NOTICE
        ;;
        3)
            logLevel=INFO
        ;;
        4)
            logLevel=TALKATIVE
        ;;
        5)
            logLevel=CHATTY
        ;;
        6)
            logLevel=DEBUG
        ;;
        7)
            logLevel=VOMIT
        ;;
        *)
            echo "_nixLogWithLevel: called with invalid log level: ${1:?}" >&"$NIX_LOG_FD";
            return 1
        ;;
    esac;
    local callerName="${FUNCNAME[2]}";
    if [[ $callerName == "_callImplicitHook" ]]; then
        callerName="${hookName:?}";
    fi;
    printf "%s: %s: %s\n" "$logLevel" "$callerName" "${2:?}" >&"$NIX_LOG_FD"
}
_overrideFirst ()
{
 
    if [ -z "${!1-}" ]; then
        _assignFirst "$@";
    fi
}
_pruneLibtoolFiles ()
{
 
    if [ "${dontPruneLibtoolFiles-}" ] || [ ! -e "$prefix" ]; then
        return;
    fi;
    find "$prefix" -type f -name '*.la' -exec grep -q '^# Generated by .*libtool' {} \; -exec grep -q "^old_library=''" {} \; -exec sed -i {} -e "/^dependency_libs='[^']/ c dependency_libs='' #pruned" \;
}
_updateSourceDateEpochFromSourceRoot ()
{
 
    if [ -n "$sourceRoot" ]; then
        updateSourceDateEpoch "$sourceRoot";
    fi
}
activatePackage ()
{
 
    local pkg="$1";
    local -r hostOffset="$2";
    local -r targetOffset="$3";
    (( hostOffset <= targetOffset )) || exit 1;
    if [ -f "$pkg" ]; then
        nixTalkativeLog "sourcing setup hook '$pkg'";
        source "$pkg";
    fi;
    if [[ -z "${strictDeps-}" || "$hostOffset" -le -1 ]]; then
        addToSearchPath _PATH "$pkg/bin";
    fi;
    if (( hostOffset <= -1 )); then
        addToSearchPath _XDG_DATA_DIRS "$pkg/share";
    fi;
    if [[ "$hostOffset" -eq 0 && -d "$pkg/bin" ]]; then
        addToSearchPath _HOST_PATH "$pkg/bin";
    fi;
    if [[ -f "$pkg/nix-support/setup-hook" ]]; then
        nixTalkativeLog "sourcing setup hook '$pkg/nix-support/setup-hook'";
        source "$pkg/nix-support/setup-hook";
    fi
}
addCMakeParams ()
{
 
    addToSearchPath NIXPKGS_CMAKE_PREFIX_PATH $1
}
addEnvHooks ()
{
 
    local depHostOffset="$1";
    shift;
    local pkgHookVarsSlice="${pkgHookVarVars[$depHostOffset + 1]}[@]";
    local pkgHookVar;
    for pkgHookVar in "${!pkgHookVarsSlice}";
    do
        eval "${pkgHookVar}s"'+=("$@")';
    done
}
addToSearchPath ()
{
 
    addToSearchPathWithCustomDelimiter ":" "$@"
}
addToSearchPathWithCustomDelimiter ()
{
 
    local delimiter="$1";
    local varName="$2";
    local dir="$3";
    if [[ -d "$dir" && "${!varName:+${delimiter}${!varName}${delimiter}}" != *"${delimiter}${dir}${delimiter}"* ]]; then
        export "${varName}=${!varName:+${!varName}${delimiter}}${dir}";
    fi
}
appendToVar ()
{
 
    local -n nameref="$1";
    local useArray type;
    if [ -n "$__structuredAttrs" ]; then
        useArray=true;
    else
        useArray=false;
    fi;
    if type=$(declare -p "$1" 2> /dev/null); then
        case "${type#* }" in 
            -A*)
                echo "appendToVar(): ERROR: trying to use appendToVar on an associative array, use variable+=([\"X\"]=\"Y\") instead." 1>&2;
                return 1
            ;;
            -a*)
                useArray=true
            ;;
            *)
                useArray=false
            ;;
        esac;
    fi;
    shift;
    if $useArray; then
        nameref=(${nameref+"${nameref[@]}"} "$@");
    else
        nameref="${nameref-} $*";
    fi
}
auditTmpdir ()
{
 
    local dir="$1";
    [ -e "$dir" ] || return 0;
    echo "checking for references to $TMPDIR/ in $dir...";
    local tmpdir elf_fifo script_fifo;
    tmpdir="$(mktemp -d)";
    elf_fifo="$tmpdir/elf";
    script_fifo="$tmpdir/script";
    mkfifo "$elf_fifo" "$script_fifo";
    ( find "$dir" -type f -not -path '*/.build-id/*' -print0 | while IFS= read -r -d '' file; do
        if isELF "$file"; then
            printf '%s\0' "$file" 1>&3;
        else
            if isScript "$file"; then
                filename=${file##*/};
                dir=${file%/*};
                if [ -e "$dir/.$filename-wrapped" ]; then
                    printf '%s\0' "$file" 1>&4;
                fi;
            fi;
        fi;
    done;
    exec 3>&- 4>&- ) 3> "$elf_fifo" 4> "$script_fifo" & ( xargs -0 -r -P "$NIX_BUILD_CORES" -n 1 sh -c '
            if { printf :; patchelf --print-rpath "$1"; } | grep -q -F ":$TMPDIR/"; then
                echo "RPATH of binary $1 contains a forbidden reference to $TMPDIR/"
                exit 1
            fi
        ' _ < "$elf_fifo" ) & local pid_elf=$!;
    local pid_script;
    ( xargs -0 -r -P "$NIX_BUILD_CORES" -n 1 sh -c '
            if grep -q -F "$TMPDIR/" "$1"; then
                echo "wrapper script $1 contains a forbidden reference to $TMPDIR/"
                exit 1
            fi
        ' _ < "$script_fifo" ) & local pid_script=$!;
    wait "$pid_elf" || { 
        echo "Some binaries contain forbidden references to $TMPDIR/. Check the error above!";
        exit 1
    };
    wait "$pid_script" || { 
        echo "Some scripts contain forbidden references to $TMPDIR/. Check the error above!";
        exit 1
    };
    rm -r "$tmpdir"
}
bintoolsWrapper_addLDVars ()
{
 
    local role_post;
    getHostRoleEnvHook;
    if [[ -d "$1/lib64" && ! -L "$1/lib64" ]]; then
        export NIX_LDFLAGS${role_post}+=" -L$1/lib64";
    fi;
    if [[ -d "$1/lib" ]]; then
        local -a glob=($1/lib/lib*);
        if [ "${#glob[*]}" -gt 0 ]; then
            export NIX_LDFLAGS${role_post}+=" -L$1/lib";
        fi;
    fi
}
buildPhase ()
{
 
    runHook preBuild;
    if [[ -z "${makeFlags-}" && -z "${makefile:-}" && ! ( -e Makefile || -e makefile || -e GNUmakefile ) ]]; then
        echo "no Makefile or custom buildPhase, doing nothing";
    else
        foundMakefile=1;
        local flagsArray=(${enableParallelBuilding:+-j${NIX_BUILD_CORES}} SHELL="$SHELL");
        concatTo flagsArray makeFlags makeFlagsArray buildFlags buildFlagsArray;
        echoCmd 'build flags' "${flagsArray[@]}";
        make ${makefile:+-f $makefile} "${flagsArray[@]}";
        unset flagsArray;
    fi;
    runHook postBuild
}
ccWrapper_addCVars ()
{
 
    local role_post;
    getHostRoleEnvHook;
    local found=;
    if [ -d "$1/include" ]; then
        export NIX_CFLAGS_COMPILE${role_post}+=" -isystem $1/include";
        found=1;
    fi;
    if [ -d "$1/Library/Frameworks" ]; then
        export NIX_CFLAGS_COMPILE${role_post}+=" -iframework $1/Library/Frameworks";
        found=1;
    fi;
    if [[ -n "" && -n ${NIX_STORE:-} && -n $found ]]; then
        local scrubbed="$NIX_STORE/eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee-${1#"$NIX_STORE"/*-}";
        export NIX_CFLAGS_COMPILE${role_post}+=" -fmacro-prefix-map=$1=$scrubbed";
    fi
}
checkPhase ()
{
 
    runHook preCheck;
    if [[ -z "${foundMakefile:-}" ]]; then
        echo "no Makefile or custom checkPhase, doing nothing";
        runHook postCheck;
        return;
    fi;
    if [[ -z "${checkTarget:-}" ]]; then
        if make -n ${makefile:+-f $makefile} check > /dev/null 2>&1; then
            checkTarget="check";
        else
            if make -n ${makefile:+-f $makefile} test > /dev/null 2>&1; then
                checkTarget="test";
            fi;
        fi;
    fi;
    if [[ -z "${checkTarget:-}" ]]; then
        echo "no check/test target in ${makefile:-Makefile}, doing nothing";
    else
        local flagsArray=(${enableParallelChecking:+-j${NIX_BUILD_CORES}} SHELL="$SHELL");
        concatTo flagsArray makeFlags makeFlagsArray checkFlags=VERBOSE=y checkFlagsArray checkTarget;
        echoCmd 'check flags' "${flagsArray[@]}";
        make ${makefile:+-f $makefile} "${flagsArray[@]}";
        unset flagsArray;
    fi;
    runHook postCheck
}
cmakeConfigurePhase ()
{
 
    runHook preConfigure;
    : ${cmakeBuildDir:=build};
    export CTEST_OUTPUT_ON_FAILURE=1;
    if [ -n "${enableParallelChecking-1}" ]; then
        export CTEST_PARALLEL_LEVEL=$NIX_BUILD_CORES;
    fi;
    if [ -z "${dontUseCmakeBuildDir-}" ]; then
        mkdir -p "$cmakeBuildDir";
        cd "$cmakeBuildDir";
        : ${cmakeDir:=..};
    else
        : ${cmakeDir:=.};
    fi;
    if [ -z "${dontAddPrefix-}" ]; then
        prependToVar cmakeFlags "-DCMAKE_INSTALL_PREFIX=$prefix";
    fi;
    prependToVar cmakeFlags "-DCMAKE_CXX_COMPILER=$CXX";
    prependToVar cmakeFlags "-DCMAKE_C_COMPILER=$CC";
    prependToVar cmakeFlags "-DCMAKE_AR=$(command -v $AR)";
    prependToVar cmakeFlags "-DCMAKE_RANLIB=$(command -v $RANLIB)";
    prependToVar cmakeFlags "-DCMAKE_STRIP=$(command -v $STRIP)";
    prependToVar cmakeFlags "-DCMAKE_INSTALL_NAME_DIR=${!outputLib}/lib";
    if [[ -z "$shareDocName" ]]; then
        local cmakeLists="${cmakeDir}/CMakeLists.txt";
        if [[ -f "$cmakeLists" ]]; then
            local shareDocName="$(grep --only-matching --perl-regexp --ignore-case '\bproject\s*\(\s*"?\K([^[:space:]")]+)' < "$cmakeLists" | head -n1)";
        fi;
        if [[ -z "$shareDocName" ]] || echo "$shareDocName" | grep -q '[^a-zA-Z0-9_+-]'; then
            if [[ -n "${pname-}" ]]; then
                shareDocName="$pname";
            else
                shareDocName="$(echo "$name" | sed 's/-[^a-zA-Z].*//')";
            fi;
        fi;
    fi;
    prependToVar cmakeFlags "-DCMAKE_INSTALL_BINDIR=${!outputBin}/bin";
    prependToVar cmakeFlags "-DCMAKE_INSTALL_SBINDIR=${!outputBin}/sbin";
    prependToVar cmakeFlags "-DCMAKE_INSTALL_INCLUDEDIR=${!outputInclude}/include";
    prependToVar cmakeFlags "-DCMAKE_INSTALL_MANDIR=${!outputMan}/share/man";
    prependToVar cmakeFlags "-DCMAKE_INSTALL_INFODIR=${!outputInfo}/share/info";
    prependToVar cmakeFlags "-DCMAKE_INSTALL_DOCDIR=${!outputDoc}/share/doc/${shareDocName}";
    prependToVar cmakeFlags "-DCMAKE_INSTALL_LIBDIR=${!outputLib}/lib";
    prependToVar cmakeFlags "-DCMAKE_INSTALL_LIBEXECDIR=${!outputLib}/libexec";
    prependToVar cmakeFlags "-DCMAKE_INSTALL_LOCALEDIR=${!outputLib}/share/locale";
    if [ -z "${doCheck-}" ]; then
        prependToVar cmakeFlags "-DBUILD_TESTING=OFF";
    fi;
    prependToVar cmakeFlags "-DCMAKE_BUILD_TYPE=${cmakeBuildType:-Release}";
    prependToVar cmakeFlags "-DCMAKE_EXPORT_NO_PACKAGE_REGISTRY=ON";
    prependToVar cmakeFlags "-DCMAKE_FIND_USE_PACKAGE_REGISTRY=OFF";
    prependToVar cmakeFlags "-DCMAKE_FIND_USE_SYSTEM_PACKAGE_REGISTRY=OFF";
    if [ "${buildPhase-}" = ninjaBuildPhase ]; then
        prependToVar cmakeFlags "-GNinja";
    fi;
    local flagsArray=();
    concatTo flagsArray cmakeFlags cmakeFlagsArray;
    echoCmd 'cmake flags' "${flagsArray[@]}";
    cmake "$cmakeDir" "${flagsArray[@]}";
    if ! [[ -v enableParallelBuilding ]]; then
        enableParallelBuilding=1;
        echo "cmake: enabled parallel building";
    fi;
    if [[ "$enableParallelBuilding" -ne 0 ]]; then
        export CMAKE_BUILD_PARALLEL_LEVEL=$NIX_BUILD_CORES;
    fi;
    if ! [[ -v enableParallelInstalling ]]; then
        enableParallelInstalling=1;
        echo "cmake: enabled parallel installing";
    fi;
    runHook postConfigure
}
cmakePcfileCheckPhase ()
{
 
    while IFS= read -rd '' file; do
        grepout=$(grep --line-number '}//nix/store' "$file" || true);
        if [ -n "$grepout" ]; then
            { 
                echo "Broken paths found in a .pc file! $file";
                echo "The following lines have issues (specifically '//' in paths).";
                echo "$grepout";
                echo "It is very likely that paths are being joined improperly.";
                echo 'ex: "${prefix}/@CMAKE_INSTALL_LIBDIR@" should be "@CMAKE_INSTALL_FULL_LIBDIR@"';
                echo "Please see https://github.com/NixOS/nixpkgs/issues/144170 for more details.";
                exit 1
            } 1>&2;
        fi;
    done < <(find "${!outputDev}" -iname "*.pc" -print0)
}
compressManPages ()
{
 
    local dir="$1";
    if [ -L "$dir"/share ] || [ -L "$dir"/share/man ] || [ ! -d "$dir/share/man" ]; then
        return;
    fi;
    echo "gzipping man pages under $dir/share/man/";
    find "$dir"/share/man/ -type f -a '!' -regex '.*\.\(bz2\|gz\|xz\)$' -print0 | xargs -0 -n1 -P "$NIX_BUILD_CORES" gzip -n -f;
    find "$dir"/share/man/ -type l -a '!' -regex '.*\.\(bz2\|gz\|xz\)$' -print0 | sort -z | while IFS= read -r -d '' f; do
        local target;
        target="$(readlink -f "$f")";
        if [ -f "$target".gz ]; then
            ln -sf "$target".gz "$f".gz && rm "$f";
        fi;
    done
}
concatStringsSep ()
{
 
    local sep="$1";
    local name="$2";
    local type oldifs;
    if type=$(declare -p "$name" 2> /dev/null); then
        local -n nameref="$name";
        case "${type#* }" in 
            -A*)
                echo "concatStringsSep(): ERROR: trying to use concatStringsSep on an associative array." 1>&2;
                return 1
            ;;
            -a*)
                local IFS="$(printf '\036')"
            ;;
            *)
                local IFS=" "
            ;;
        esac;
        local ifs_separated="${nameref[*]}";
        echo -n "${ifs_separated//"$IFS"/"$sep"}";
    fi
}
concatTo ()
{
 
    local -;
    set -o noglob;
    local -n targetref="$1";
    shift;
    local arg default name type;
    for arg in "$@";
    do
        IFS="=" read -r name default <<< "$arg";
        local -n nameref="$name";
        if [[ -z "${nameref[*]}" && -n "$default" ]]; then
            targetref+=("$default");
        else
            if type=$(declare -p "$name" 2> /dev/null); then
                case "${type#* }" in 
                    -A*)
                        echo "concatTo(): ERROR: trying to use concatTo on an associative array." 1>&2;
                        return 1
                    ;;
                    -a*)
                        targetref+=("${nameref[@]}")
                    ;;
                    *)
                        if [[ "$name" = *"Array" ]]; then
                            nixErrorLog "concatTo(): $name is not declared as array, treating as a singleton. This will become an error in future";
                            targetref+=(${nameref+"${nameref[@]}"});
                        else
                            targetref+=(${nameref-});
                        fi
                    ;;
                esac;
            fi;
        fi;
    done
}
configurePhase ()
{
 
    runHook preConfigure;
    : "${configureScript=}";
    if [[ -z "$configureScript" && -x ./configure ]]; then
        configureScript=./configure;
    fi;
    if [ -z "${dontFixLibtool:-}" ]; then
        export lt_cv_deplibs_check_method="${lt_cv_deplibs_check_method-pass_all}";
        local i;
        find . -iname "ltmain.sh" -print0 | while IFS='' read -r -d '' i; do
            echo "fixing libtool script $i";
            fixLibtool "$i";
        done;
        CONFIGURE_MTIME_REFERENCE=$(mktemp configure.mtime.reference.XXXXXX);
        find . -executable -type f -name configure -exec grep -l 'GNU Libtool is free software; you can redistribute it and/or modify' {} \; -exec touch -r {} "$CONFIGURE_MTIME_REFERENCE" \; -exec sed -i s_/usr/bin/file_file_g {} \; -exec touch -r "$CONFIGURE_MTIME_REFERENCE" {} \;;
        rm -f "$CONFIGURE_MTIME_REFERENCE";
    fi;
    if [[ -z "${dontAddPrefix:-}" && -n "$prefix" ]]; then
        prependToVar configureFlags "${prefixKey:---prefix=}$prefix";
    fi;
    if [[ -f "$configureScript" ]]; then
        if [ -z "${dontAddDisableDepTrack:-}" ]; then
            if grep -q dependency-tracking "$configureScript"; then
                prependToVar configureFlags --disable-dependency-tracking;
            fi;
        fi;
        if [ -z "${dontDisableStatic:-}" ]; then
            if grep -q enable-static "$configureScript"; then
                prependToVar configureFlags --disable-static;
            fi;
        fi;
        if [ -z "${dontPatchShebangsInConfigure:-}" ]; then
            patchShebangs --build "$configureScript";
        fi;
    fi;
    if [ -n "$configureScript" ]; then
        local -a flagsArray;
        concatTo flagsArray configureFlags configureFlagsArray;
        echoCmd 'configure flags' "${flagsArray[@]}";
        $configureScript "${flagsArray[@]}";
        unset flagsArray;
    else
        echo "no configure script, doing nothing";
    fi;
    runHook postConfigure
}
consumeEntire ()
{
 
    if IFS='' read -r -d '' "$1"; then
        echo "consumeEntire(): ERROR: Input null bytes, won't process" 1>&2;
        return 1;
    fi
}
definePhases ()
{
 
    if [ -z "${phases[*]:-}" ]; then
        phases="${prePhases[*]:-} unpackPhase patchPhase ${preConfigurePhases[*]:-}             configurePhase ${preBuildPhases[*]:-} buildPhase checkPhase             ${preInstallPhases[*]:-} installPhase ${preFixupPhases[*]:-} fixupPhase installCheckPhase             ${preDistPhases[*]:-} distPhase ${postPhases[*]:-}";
    fi
}
distPhase ()
{
 
    runHook preDist;
    local flagsArray=();
    concatTo flagsArray distFlags distFlagsArray distTarget=dist;
    echo 'dist flags: %q' "${flagsArray[@]}";
    make ${makefile:+-f $makefile} "${flagsArray[@]}";
    if [ "${dontCopyDist:-0}" != 1 ]; then
        mkdir -p "$out/tarballs";
        cp -pvd ${tarballs[*]:-*.tar.gz} "$out/tarballs";
    fi;
    runHook postDist
}
dumpVars ()
{
 
    if [[ "${noDumpEnvVars:-0}" != 1 && -d "$NIX_BUILD_TOP" ]]; then
        local old_umask;
        old_umask=$(umask);
        umask 0077;
        export 2> /dev/null > "$NIX_BUILD_TOP/env-vars";
        umask "$old_umask";
    fi
}
echoCmd ()
{
 
    printf "%s:" "$1";
    shift;
    printf ' %q' "$@";
    echo
}
exitHandler ()
{
 
    exitCode="$?";
    set +e;
    if [ -n "${showBuildStats:-}" ]; then
        read -r -d '' -a buildTimes < <(times);
        echo "build times:";
        echo "user time for the shell             ${buildTimes[0]}";
        echo "system time for the shell           ${buildTimes[1]}";
        echo "user time for all child processes   ${buildTimes[2]}";
        echo "system time for all child processes ${buildTimes[3]}";
    fi;
    if (( "$exitCode" != 0 )); then
        runHook failureHook;
        if [ -n "${succeedOnFailure:-}" ]; then
            echo "build failed with exit code $exitCode (ignored)";
            mkdir -p "$out/nix-support";
            printf "%s" "$exitCode" > "$out/nix-support/failed";
            exit 0;
        fi;
    else
        runHook exitHook;
    fi;
    return "$exitCode"
}
findGdkPixbufLoaders ()
{
 
    local loadersCache="$1/lib/gdk-pixbuf-2.0/2.10.0/loaders.cache";
    if [[ -f "$loadersCache" ]]; then
        if [[ -f "${GDK_PIXBUF_MODULE_FILE-}" ]]; then
            if (( "$(cat "$loadersCache" | wc -l)" > "$(cat "$GDK_PIXBUF_MODULE_FILE" | wc -l)" )); then
                export GDK_PIXBUF_MODULE_FILE="$loadersCache";
            fi;
        else
            export GDK_PIXBUF_MODULE_FILE="$loadersCache";
        fi;
    fi
}
findInputs ()
{
 
    local -r pkg="$1";
    local -r hostOffset="$2";
    local -r targetOffset="$3";
    (( hostOffset <= targetOffset )) || exit 1;
    local varVar="${pkgAccumVarVars[hostOffset + 1]}";
    local varRef="$varVar[$((targetOffset - hostOffset))]";
    local var="${!varRef}";
    unset -v varVar varRef;
    local varSlice="$var[*]";
    case " ${!varSlice-} " in 
        *" $pkg "*)
            return 0
        ;;
    esac;
    unset -v varSlice;
    eval "$var"'+=("$pkg")';
    if ! [ -e "$pkg" ]; then
        echo "build input $pkg does not exist" 1>&2;
        exit 1;
    fi;
    function mapOffset () 
    { 
        local -r inputOffset="$1";
        local -n outputOffset="$2";
        if (( inputOffset <= 0 )); then
            outputOffset=$((inputOffset + hostOffset));
        else
            outputOffset=$((inputOffset - 1 + targetOffset));
        fi
    };
    local relHostOffset;
    for relHostOffset in "${allPlatOffsets[@]}";
    do
        local files="${propagatedDepFilesVars[relHostOffset + 1]}";
        local hostOffsetNext;
        mapOffset "$relHostOffset" hostOffsetNext;
        (( -1 <= hostOffsetNext && hostOffsetNext <= 1 )) || continue;
        local relTargetOffset;
        for relTargetOffset in "${allPlatOffsets[@]}";
        do
            (( "$relHostOffset" <= "$relTargetOffset" )) || continue;
            local fileRef="${files}[$relTargetOffset - $relHostOffset]";
            local file="${!fileRef}";
            unset -v fileRef;
            local targetOffsetNext;
            mapOffset "$relTargetOffset" targetOffsetNext;
            (( -1 <= hostOffsetNext && hostOffsetNext <= 1 )) || continue;
            [[ -f "$pkg/nix-support/$file" ]] || continue;
            local pkgNext;
            read -r -d '' pkgNext < "$pkg/nix-support/$file" || true;
            for pkgNext in $pkgNext;
            do
                findInputs "$pkgNext" "$hostOffsetNext" "$targetOffsetNext";
            done;
        done;
    done
}
fixLibtool ()
{
 
    local search_path;
    for flag in $NIX_LDFLAGS;
    do
        case $flag in 
            -L*)
                search_path+=" ${flag#-L}"
            ;;
        esac;
    done;
    sed -i "$1" -e "s^eval \(sys_lib_search_path=\).*^\1'${search_path:-}'^" -e 's^eval sys_lib_.+search_path=.*^^'
}
fixupPhase ()
{
 
    local output;
    for output in $(getAllOutputNames);
    do
        if [ -e "${!output}" ]; then
            chmod -R u+w,u-s,g-s "${!output}";
        fi;
    done;
    runHook preFixup;
    local output;
    for output in $(getAllOutputNames);
    do
        prefix="${!output}" runHook fixupOutput;
    done;
    recordPropagatedDependencies;
    if [ -n "${setupHook:-}" ]; then
        mkdir -p "${!outputDev}/nix-support";
        substituteAll "$setupHook" "${!outputDev}/nix-support/setup-hook";
    fi;
    if [ -n "${setupHooks:-}" ]; then
        mkdir -p "${!outputDev}/nix-support";
        local hook;
        for hook in ${setupHooks[@]};
        do
            local content;
            consumeEntire content < "$hook";
            substituteAllStream content "file '$hook'" >> "${!outputDev}/nix-support/setup-hook";
            unset -v content;
        done;
        unset -v hook;
    fi;
    if [ -n "${propagatedUserEnvPkgs[*]:-}" ]; then
        mkdir -p "${!outputBin}/nix-support";
        printWords "${propagatedUserEnvPkgs[@]}" > "${!outputBin}/nix-support/propagated-user-env-packages";
    fi;
    runHook postFixup
}
genericBuild ()
{
 
    export GZIP_NO_TIMESTAMPS=1;
    if [ -f "${buildCommandPath:-}" ]; then
        source "$buildCommandPath";
        return;
    fi;
    if [ -n "${buildCommand:-}" ]; then
        eval "$buildCommand";
        return;
    fi;
    definePhases;
    for curPhase in ${phases[*]};
    do
        runPhase "$curPhase";
    done
}
getAllOutputNames ()
{
 
    if [ -n "$__structuredAttrs" ]; then
        echo "${!outputs[*]}";
    else
        echo "$outputs";
    fi
}
getHostRole ()
{
 
    getRole "$hostOffset"
}
getHostRoleEnvHook ()
{
 
    getRole "$depHostOffset"
}
getRole ()
{
 
    case $1 in 
        -1)
            role_post='_FOR_BUILD'
        ;;
        0)
            role_post=''
        ;;
        1)
            role_post='_FOR_TARGET'
        ;;
        *)
            echo "gettext-0.25.1: used as improper sort of dependency" 1>&2;
            return 1
        ;;
    esac
}
getTargetRole ()
{
 
    getRole "$targetOffset"
}
getTargetRoleEnvHook ()
{
 
    getRole "$depTargetOffset"
}
getTargetRoleWrapper ()
{
 
    case $targetOffset in 
        -1)
            export NIX_@wrapperName@_TARGET_BUILD_@suffixSalt@=1
        ;;
        0)
            export NIX_@wrapperName@_TARGET_HOST_@suffixSalt@=1
        ;;
        1)
            export NIX_@wrapperName@_TARGET_TARGET_@suffixSalt@=1
        ;;
        *)
            echo "gettext-0.25.1: used as improper sort of dependency" 1>&2;
            return 1
        ;;
    esac
}
gettextDataDirsHook ()
{
 
    getHostRoleEnvHook;
    if [ -d "$1/share/gettext" ]; then
        addToSearchPath "GETTEXTDATADIRS${role_post}" "$1/share/gettext";
    fi
}
glibPostInstallHook ()
{
 
    if [ -d "$prefix/share/glib-2.0/schemas" ]; then
        mkdir -p "${!outputLib}/share/gsettings-schemas/$name/glib-2.0";
        mv "$prefix/share/glib-2.0/schemas" "${!outputLib}/share/gsettings-schemas/$name/glib-2.0/";
    fi;
    addToSearchPath GSETTINGS_SCHEMAS_PATH "${!outputLib}/share/gsettings-schemas/$name"
}
glibPreInstallPhase ()
{
 
    makeFlagsArray+=("gsettingsschemadir=${!outputLib}/share/gsettings-schemas/$name/glib-2.0/schemas/")
}
installCheckPhase ()
{
 
    runHook preInstallCheck;
    if [[ -z "${foundMakefile:-}" ]]; then
        echo "no Makefile or custom installCheckPhase, doing nothing";
    else
        if [[ -z "${installCheckTarget:-}" ]] && ! make -n ${makefile:+-f $makefile} "${installCheckTarget:-installcheck}" > /dev/null 2>&1; then
            echo "no installcheck target in ${makefile:-Makefile}, doing nothing";
        else
            local flagsArray=(${enableParallelChecking:+-j${NIX_BUILD_CORES}} SHELL="$SHELL");
            concatTo flagsArray makeFlags makeFlagsArray installCheckFlags installCheckFlagsArray installCheckTarget=installcheck;
            echoCmd 'installcheck flags' "${flagsArray[@]}";
            make ${makefile:+-f $makefile} "${flagsArray[@]}";
            unset flagsArray;
        fi;
    fi;
    runHook postInstallCheck
}
installPhase ()
{
 
    runHook preInstall;
    if [[ -z "${makeFlags-}" && -z "${makefile:-}" && ! ( -e Makefile || -e makefile || -e GNUmakefile ) ]]; then
        echo "no Makefile or custom installPhase, doing nothing";
        runHook postInstall;
        return;
    else
        foundMakefile=1;
    fi;
    if [ -n "$prefix" ]; then
        mkdir -p "$prefix";
    fi;
    local flagsArray=(${enableParallelInstalling:+-j${NIX_BUILD_CORES}} SHELL="$SHELL");
    concatTo flagsArray makeFlags makeFlagsArray installFlags installFlagsArray installTargets=install;
    echoCmd 'install flags' "${flagsArray[@]}";
    make ${makefile:+-f $makefile} "${flagsArray[@]}";
    unset flagsArray;
    runHook postInstall
}
isELF ()
{
 
    local fn="$1";
    local fd;
    local magic;
    exec {fd}< "$fn";
    LANG=C read -r -n 4 -u "$fd" magic;
    exec {fd}>&-;
    if [ "$magic" = 'ELF' ]; then
        return 0;
    else
        return 1;
    fi
}
isMachO ()
{
 
    local fn="$1";
    local fd;
    local magic;
    exec {fd}< "$fn";
    LANG=C read -r -n 4 -u "$fd" magic;
    exec {fd}>&-;
    if [[ "$magic" = $(echo -ne "\xfe\xed\xfa\xcf") || "$magic" = $(echo -ne "\xcf\xfa\xed\xfe") ]]; then
        return 0;
    else
        if [[ "$magic" = $(echo -ne "\xfe\xed\xfa\xce") || "$magic" = $(echo -ne "\xce\xfa\xed\xfe") ]]; then
            return 0;
        else
            if [[ "$magic" = $(echo -ne "\xca\xfe\xba\xbe") || "$magic" = $(echo -ne "\xbe\xba\xfe\xca") ]]; then
                return 0;
            else
                return 1;
            fi;
        fi;
    fi
}
isScript ()
{
 
    local fn="$1";
    local fd;
    local magic;
    exec {fd}< "$fn";
    LANG=C read -r -n 2 -u "$fd" magic;
    exec {fd}>&-;
    if [[ "$magic" =~ \#! ]]; then
        return 0;
    else
        return 1;
    fi
}
makeCmakeFindLibs ()
{
 
    isystem_seen=;
    iframework_seen=;
    for flag in ${NIX_CFLAGS_COMPILE-} ${NIX_LDFLAGS-};
    do
        if test -n "$isystem_seen" && test -d "$flag"; then
            isystem_seen=;
            addToSearchPath CMAKE_INCLUDE_PATH "${flag}";
        else
            if test -n "$iframework_seen" && test -d "$flag"; then
                iframework_seen=;
                addToSearchPath CMAKE_FRAMEWORK_PATH "${flag}";
            else
                isystem_seen=;
                iframework_seen=;
                case $flag in 
                    -I*)
                        addToSearchPath CMAKE_INCLUDE_PATH "${flag:2}"
                    ;;
                    -L*)
                        addToSearchPath CMAKE_LIBRARY_PATH "${flag:2}"
                    ;;
                    -F*)
                        addToSearchPath CMAKE_FRAMEWORK_PATH "${flag:2}"
                    ;;
                    -isystem)
                        isystem_seen=1
                    ;;
                    -iframework)
                        iframework_seen=1
                    ;;
                esac;
            fi;
        fi;
    done
}
make_glib_find_gsettings_schemas ()
{
 
    for maybe_dir in "$1"/share/gsettings-schemas/*;
    do
        if [[ -d "$maybe_dir/glib-2.0/schemas" ]]; then
            addToSearchPath GSETTINGS_SCHEMAS_PATH "$maybe_dir";
        fi;
    done
}
mapOffset ()
{
 
    local -r inputOffset="$1";
    local -n outputOffset="$2";
    if (( inputOffset <= 0 )); then
        outputOffset=$((inputOffset + hostOffset));
    else
        outputOffset=$((inputOffset - 1 + targetOffset));
    fi
}
moveToOutput ()
{
 
    local patt="$1";
    local dstOut="$2";
    local output;
    for output in $(getAllOutputNames);
    do
        if [ "${!output}" = "$dstOut" ]; then
            continue;
        fi;
        local srcPath;
        for srcPath in "${!output}"/$patt;
        do
            if [ ! -e "$srcPath" ] && [ ! -L "$srcPath" ]; then
                continue;
            fi;
            if [ "$dstOut" = REMOVE ]; then
                echo "Removing $srcPath";
                rm -r "$srcPath";
            else
                local dstPath="$dstOut${srcPath#${!output}}";
                echo "Moving $srcPath to $dstPath";
                if [ -d "$dstPath" ] && [ -d "$srcPath" ]; then
                    rmdir "$srcPath" --ignore-fail-on-non-empty;
                    if [ -d "$srcPath" ]; then
                        mv -t "$dstPath" "$srcPath"/*;
                        rmdir "$srcPath";
                    fi;
                else
                    mkdir -p "$(readlink -m "$dstPath/..")";
                    mv "$srcPath" "$dstPath";
                fi;
            fi;
            local srcParent="$(readlink -m "$srcPath/..")";
            if [ -n "$(find "$srcParent" -maxdepth 0 -type d -empty 2> /dev/null)" ]; then
                echo "Removing empty $srcParent/ and (possibly) its parents";
                rmdir -p --ignore-fail-on-non-empty "$srcParent" 2> /dev/null || true;
            fi;
        done;
    done
}
ninjaBuildPhase ()
{
 
    runHook preBuild;
    local buildCores=1;
    if [ "${enableParallelBuilding-1}" ]; then
        buildCores="$NIX_BUILD_CORES";
    fi;
    local flagsArray=("-j$buildCores");
    concatTo flagsArray ninjaFlags ninjaFlagsArray;
    echoCmd 'build flags' "${flagsArray[@]}";
    TERM=dumb ninja "${flagsArray[@]}";
    runHook postBuild
}
ninjaCheckPhase ()
{
 
    runHook preCheck;
    if [ -z "${checkTarget:-}" ]; then
        if ninja -t query test > /dev/null 2>&1; then
            checkTarget="test";
        fi;
    fi;
    if [ -z "${checkTarget:-}" ]; then
        echo "no test target found in ninja, doing nothing";
    else
        local buildCores=1;
        if [ "${enableParallelChecking-1}" ]; then
            buildCores="$NIX_BUILD_CORES";
        fi;
        local flagsArray=("-j$buildCores");
        concatTo flagsArray ninjaFlags ninjaFlagsArray checkTarget;
        echoCmd 'check flags' "${flagsArray[@]}";
        TERM=dumb ninja "${flagsArray[@]}";
    fi;
    runHook postCheck
}
ninjaInstallPhase ()
{
 
    runHook preInstall;
    local buildCores=1;
    if [ "${enableParallelInstalling-1}" ]; then
        buildCores="$NIX_BUILD_CORES";
    fi;
    local flagsArray=("-j$buildCores");
    concatTo flagsArray ninjaFlags ninjaFlagsArray installTargets=install;
    echoCmd 'install flags' "${flagsArray[@]}";
    TERM=dumb ninja "${flagsArray[@]}";
    runHook postInstall
}
nixChattyLog ()
{
 
    _nixLogWithLevel 5 "$*"
}
nixDebugLog ()
{
 
    _nixLogWithLevel 6 "$*"
}
nixErrorLog ()
{
 
    _nixLogWithLevel 0 "$*"
}
nixInfoLog ()
{
 
    _nixLogWithLevel 3 "$*"
}
nixLog ()
{
 
    [[ -z ${NIX_LOG_FD-} ]] && return 0;
    local callerName="${FUNCNAME[1]}";
    if [[ $callerName == "_callImplicitHook" ]]; then
        callerName="${hookName:?}";
    fi;
    printf "%s: %s\n" "$callerName" "$*" >&"$NIX_LOG_FD"
}
nixNoticeLog ()
{
 
    _nixLogWithLevel 2 "$*"
}
nixTalkativeLog ()
{
 
    _nixLogWithLevel 4 "$*"
}
nixVomitLog ()
{
 
    _nixLogWithLevel 7 "$*"
}
nixWarnLog ()
{
 
    _nixLogWithLevel 1 "$*"
}
noBrokenSymlinks ()
{
 
    local -r output="${1:?}";
    local path;
    local pathParent;
    local symlinkTarget;
    local -i numDanglingSymlinks=0;
    local -i numReflexiveSymlinks=0;
    local -i numUnreadableSymlinks=0;
    if [[ ! -e $output ]]; then
        nixWarnLog "skipping non-existent output $output";
        return 0;
    fi;
    nixInfoLog "running on $output";
    while IFS= read -r -d '' path; do
        pathParent="$(dirname "$path")";
        if ! symlinkTarget="$(readlink "$path")"; then
            nixErrorLog "the symlink $path is unreadable";
            numUnreadableSymlinks+=1;
            continue;
        fi;
        if [[ $symlinkTarget == /* ]]; then
            nixInfoLog "symlink $path points to absolute target $symlinkTarget";
        else
            nixInfoLog "symlink $path points to relative target $symlinkTarget";
            symlinkTarget="$(realpath --no-symlinks --canonicalize-missing "$pathParent/$symlinkTarget")";
        fi;
        if [[ $symlinkTarget = "$TMPDIR"/* ]]; then
            nixErrorLog "the symlink $path points to $TMPDIR directory: $symlinkTarget";
            numDanglingSymlinks+=1;
            continue;
        fi;
        if [[ $symlinkTarget != "$NIX_STORE"/* ]]; then
            nixInfoLog "symlink $path points outside the Nix store; ignoring";
            continue;
        fi;
        if [[ $path == "$symlinkTarget" ]]; then
            nixErrorLog "the symlink $path is reflexive";
            numReflexiveSymlinks+=1;
        else
            if [[ ! -e $symlinkTarget ]]; then
                nixErrorLog "the symlink $path points to a missing target: $symlinkTarget";
                numDanglingSymlinks+=1;
            else
                nixDebugLog "the symlink $path is irreflexive and points to a target which exists";
            fi;
        fi;
    done < <(find "$output" -type l -print0);
    if ((numDanglingSymlinks > 0 || numReflexiveSymlinks > 0 || numUnreadableSymlinks > 0)); then
        nixErrorLog "found $numDanglingSymlinks dangling symlinks, $numReflexiveSymlinks reflexive symlinks and $numUnreadableSymlinks unreadable symlinks";
        exit 1;
    fi;
    return 0
}
noBrokenSymlinksInAllOutputs ()
{
 
    if [[ -z ${dontCheckForBrokenSymlinks-} ]]; then
        for output in $(getAllOutputNames);
        do
            noBrokenSymlinks "${!output}";
        done;
    fi
}
patchELF ()
{
 
    local dir="$1";
    [ -e "$dir" ] || return 0;
    echo "shrinking RPATHs of ELF executables and libraries in $dir";
    local i;
    while IFS= read -r -d '' i; do
        if [[ "$i" =~ .build-id ]]; then
            continue;
        fi;
        if ! isELF "$i"; then
            continue;
        fi;
        echo "shrinking $i";
        patchelf --shrink-rpath "$i" || true;
    done < <(find "$dir" -type f -print0)
}
patchPhase ()
{
 
    runHook prePatch;
    local -a patchesArray;
    concatTo patchesArray patches;
    local -a flagsArray;
    concatTo flagsArray patchFlags=-p1;
    for i in "${patchesArray[@]}";
    do
        echo "applying patch $i";
        local uncompress=cat;
        case "$i" in 
            *.gz)
                uncompress="gzip -d"
            ;;
            *.bz2)
                uncompress="bzip2 -d"
            ;;
            *.xz)
                uncompress="xz -d"
            ;;
            *.lzma)
                uncompress="lzma -d"
            ;;
        esac;
        $uncompress < "$i" 2>&1 | patch "${flagsArray[@]}";
    done;
    runHook postPatch
}
patchShebangs ()
{
 
    local pathName;
    local update=false;
    while [[ $# -gt 0 ]]; do
        case "$1" in 
            --host)
                pathName=HOST_PATH;
                shift
            ;;
            --build)
                pathName=PATH;
                shift
            ;;
            --update)
                update=true;
                shift
            ;;
            --)
                shift;
                break
            ;;
            -* | --*)
                echo "Unknown option $1 supplied to patchShebangs" 1>&2;
                return 1
            ;;
            *)
                break
            ;;
        esac;
    done;
    echo "patching script interpreter paths in $@";
    local f;
    local oldPath;
    local newPath;
    local arg0;
    local args;
    local oldInterpreterLine;
    local newInterpreterLine;
    if [[ $# -eq 0 ]]; then
        echo "No arguments supplied to patchShebangs" 1>&2;
        return 0;
    fi;
    local f;
    while IFS= read -r -d '' f; do
        isScript "$f" || continue;
        read -r oldInterpreterLine < "$f" || [ "$oldInterpreterLine" ];
        read -r oldPath arg0 args <<< "${oldInterpreterLine:2}";
        if [[ -z "${pathName:-}" ]]; then
            if [[ -n $strictDeps && $f == "$NIX_STORE"* ]]; then
                pathName=HOST_PATH;
            else
                pathName=PATH;
            fi;
        fi;
        if [[ "$oldPath" == *"/bin/env" ]]; then
            if [[ $arg0 == "-S" ]]; then
                arg0=${args%% *};
                [[ "$args" == *" "* ]] && args=${args#* } || args=;
                newPath="$(PATH="${!pathName}" type -P "env" || true)";
                args="-S $(PATH="${!pathName}" type -P "$arg0" || true) $args";
            else
                if [[ $arg0 == "-"* || $arg0 == *"="* ]]; then
                    echo "$f: unsupported interpreter directive \"$oldInterpreterLine\" (set dontPatchShebangs=1 and handle shebang patching yourself)" 1>&2;
                    exit 1;
                else
                    newPath="$(PATH="${!pathName}" type -P "$arg0" || true)";
                fi;
            fi;
        else
            if [[ -z $oldPath ]]; then
                oldPath="/bin/sh";
            fi;
            newPath="$(PATH="${!pathName}" type -P "$(basename "$oldPath")" || true)";
            args="$arg0 $args";
        fi;
        newInterpreterLine="$newPath $args";
        newInterpreterLine=${newInterpreterLine%${newInterpreterLine##*[![:space:]]}};
        if [[ -n "$oldPath" && ( "$update" == true || "${oldPath:0:${#NIX_STORE}}" != "$NIX_STORE" ) ]]; then
            if [[ -n "$newPath" && "$newPath" != "$oldPath" ]]; then
                echo "$f: interpreter directive changed from \"$oldInterpreterLine\" to \"$newInterpreterLine\"";
                escapedInterpreterLine=${newInterpreterLine//\\/\\\\};
                timestamp=$(stat --printf "%y" "$f");
                tmpFile=$(mktemp -t patchShebangs.XXXXXXXXXX);
                sed -e "1 s|.*|#\!$escapedInterpreterLine|" "$f" > "$tmpFile";
                local restoreReadOnly;
                if [[ ! -w "$f" ]]; then
                    chmod +w "$f";
                    restoreReadOnly=true;
                fi;
                cat "$tmpFile" > "$f";
                rm "$tmpFile";
                if [[ -n "${restoreReadOnly:-}" ]]; then
                    chmod -w "$f";
                fi;
                touch --date "$timestamp" "$f";
            fi;
        fi;
    done < <(find "$@" -type f -perm -0100 -print0)
}
patchShebangsAuto ()
{
 
    if [[ -z "${dontPatchShebangs-}" && -e "$prefix" ]]; then
        if [[ "$output" != out && "$output" = "$outputDev" ]]; then
            patchShebangs --build "$prefix";
        else
            patchShebangs --host "$prefix";
        fi;
    fi
}
pkgConfigWrapper_addPkgConfigPath ()
{
 
    local role_post;
    getHostRoleEnvHook;
    addToSearchPath "PKG_CONFIG_PATH${role_post}" "$1/lib/pkgconfig";
    addToSearchPath "PKG_CONFIG_PATH${role_post}" "$1/share/pkgconfig"
}
prependToVar ()
{
 
    local -n nameref="$1";
    local useArray type;
    if [ -n "$__structuredAttrs" ]; then
        useArray=true;
    else
        useArray=false;
    fi;
    if type=$(declare -p "$1" 2> /dev/null); then
        case "${type#* }" in 
            -A*)
                echo "prependToVar(): ERROR: trying to use prependToVar on an associative array." 1>&2;
                return 1
            ;;
            -a*)
                useArray=true
            ;;
            *)
                useArray=false
            ;;
        esac;
    fi;
    shift;
    if $useArray; then
        nameref=("$@" ${nameref+"${nameref[@]}"});
    else
        nameref="$* ${nameref-}";
    fi
}
printLines ()
{
 
    (( "$#" > 0 )) || return 0;
    printf '%s\n' "$@"
}
printPhases ()
{
 
    definePhases;
    local phase;
    for phase in ${phases[*]};
    do
        printf '%s\n' "$phase";
    done
}
printWords ()
{
 
    (( "$#" > 0 )) || return 0;
    printf '%s ' "$@"
}
recordPropagatedDependencies ()
{
 
    declare -ra flatVars=(depsBuildBuildPropagated propagatedNativeBuildInputs depsBuildTargetPropagated depsHostHostPropagated propagatedBuildInputs depsTargetTargetPropagated);
    declare -ra flatFiles=("${propagatedBuildDepFiles[@]}" "${propagatedHostDepFiles[@]}" "${propagatedTargetDepFiles[@]}");
    local propagatedInputsIndex;
    for propagatedInputsIndex in "${!flatVars[@]}";
    do
        local propagatedInputsSlice="${flatVars[$propagatedInputsIndex]}[@]";
        local propagatedInputsFile="${flatFiles[$propagatedInputsIndex]}";
        [[ -n "${!propagatedInputsSlice}" ]] || continue;
        mkdir -p "${!outputDev}/nix-support";
        printWords ${!propagatedInputsSlice} > "${!outputDev}/nix-support/$propagatedInputsFile";
    done
}
runHook ()
{
 
    local hookName="$1";
    shift;
    local hooksSlice="${hookName%Hook}Hooks[@]";
    local hook;
    for hook in "_callImplicitHook 0 $hookName" ${!hooksSlice+"${!hooksSlice}"};
    do
        _logHook "$hookName" "$hook" "$@";
        _eval "$hook" "$@";
    done;
    return 0
}
runOneHook ()
{
 
    local hookName="$1";
    shift;
    local hooksSlice="${hookName%Hook}Hooks[@]";
    local hook ret=1;
    for hook in "_callImplicitHook 1 $hookName" ${!hooksSlice+"${!hooksSlice}"};
    do
        _logHook "$hookName" "$hook" "$@";
        if _eval "$hook" "$@"; then
            ret=0;
            break;
        fi;
    done;
    return "$ret"
}
runPhase ()
{
 
    local curPhase="$*";
    if [[ "$curPhase" = unpackPhase && -n "${dontUnpack:-}" ]]; then
        return;
    fi;
    if [[ "$curPhase" = patchPhase && -n "${dontPatch:-}" ]]; then
        return;
    fi;
    if [[ "$curPhase" = configurePhase && -n "${dontConfigure:-}" ]]; then
        return;
    fi;
    if [[ "$curPhase" = buildPhase && -n "${dontBuild:-}" ]]; then
        return;
    fi;
    if [[ "$curPhase" = checkPhase && -z "${doCheck:-}" ]]; then
        return;
    fi;
    if [[ "$curPhase" = installPhase && -n "${dontInstall:-}" ]]; then
        return;
    fi;
    if [[ "$curPhase" = fixupPhase && -n "${dontFixup:-}" ]]; then
        return;
    fi;
    if [[ "$curPhase" = installCheckPhase && -z "${doInstallCheck:-}" ]]; then
        return;
    fi;
    if [[ "$curPhase" = distPhase && -z "${doDist:-}" ]]; then
        return;
    fi;
    showPhaseHeader "$curPhase";
    dumpVars;
    local startTime endTime;
    startTime=$(date +"%s");
    eval "${!curPhase:-$curPhase}";
    endTime=$(date +"%s");
    showPhaseFooter "$curPhase" "$startTime" "$endTime";
    if [ "$curPhase" = unpackPhase ]; then
        [ -n "${sourceRoot:-}" ] && chmod +x -- "${sourceRoot}";
        cd -- "${sourceRoot:-.}";
    fi
}
showPhaseFooter ()
{
 
    local phase="$1";
    local startTime="$2";
    local endTime="$3";
    local delta=$(( endTime - startTime ));
    (( delta < 30 )) && return;
    local H=$((delta/3600));
    local M=$((delta%3600/60));
    local S=$((delta%60));
    echo -n "$phase completed in ";
    (( H > 0 )) && echo -n "$H hours ";
    (( M > 0 )) && echo -n "$M minutes ";
    echo "$S seconds"
}
showPhaseHeader ()
{
 
    local phase="$1";
    echo "Running phase: $phase";
    if [[ -z ${NIX_LOG_FD-} ]]; then
        return;
    fi;
    printf "@nix { \"action\": \"setPhase\", \"phase\": \"%s\" }\n" "$phase" >&"$NIX_LOG_FD"
}
stripDirs ()
{
 
    local cmd="$1";
    local ranlibCmd="$2";
    local paths="$3";
    local stripFlags="$4";
    local excludeFlags=();
    local pathsNew=;
    [ -z "$cmd" ] && echo "stripDirs: Strip command is empty" 1>&2 && exit 1;
    [ -z "$ranlibCmd" ] && echo "stripDirs: Ranlib command is empty" 1>&2 && exit 1;
    local pattern;
    if [ -n "${stripExclude:-}" ]; then
        for pattern in "${stripExclude[@]}";
        do
            excludeFlags+=(-a '!' '(' -name "$pattern" -o -wholename "$prefix/$pattern" ')');
        done;
    fi;
    local p;
    for p in ${paths};
    do
        if [ -e "$prefix/$p" ]; then
            pathsNew="${pathsNew} $prefix/$p";
        fi;
    done;
    paths=${pathsNew};
    if [ -n "${paths}" ]; then
        echo "stripping (with command $cmd and flags $stripFlags) in $paths";
        local striperr;
        striperr="$(mktemp --tmpdir="$TMPDIR" 'striperr.XXXXXX')";
        find $paths -type f "${excludeFlags[@]}" -a '!' -path "$prefix/lib/debug/*" -printf '%D-%i,%p\0' | sort -t, -k1,1 -u -z | cut -d, -f2- -z | xargs -r -0 -n1 -P "$NIX_BUILD_CORES" -- $cmd $stripFlags 2> "$striperr" || exit_code=$?;
        [[ "$exit_code" = 123 || -z "$exit_code" ]] || ( cat "$striperr" 1>&2 && exit 1 );
        rm "$striperr";
        find $paths -name '*.a' -type f -exec $ranlibCmd '{}' \; 2> /dev/null;
    fi
}
stripHash ()
{
 
    local strippedName casematchOpt=0;
    strippedName="$(basename -- "$1")";
    shopt -q nocasematch && casematchOpt=1;
    shopt -u nocasematch;
    if [[ "$strippedName" =~ ^[a-z0-9]{32}- ]]; then
        echo "${strippedName:33}";
    else
        echo "$strippedName";
    fi;
    if (( casematchOpt )); then
        shopt -s nocasematch;
    fi
}
substitute ()
{
 
    local input="$1";
    local output="$2";
    shift 2;
    if [ ! -f "$input" ]; then
        echo "substitute(): ERROR: file '$input' does not exist" 1>&2;
        return 1;
    fi;
    local content;
    consumeEntire content < "$input";
    if [ -e "$output" ]; then
        chmod +w "$output";
    fi;
    substituteStream content "file '$input'" "$@" > "$output"
}
substituteAll ()
{
 
    local input="$1";
    local output="$2";
    local -a args=();
    _allFlags;
    substitute "$input" "$output" "${args[@]}"
}
substituteAllInPlace ()
{
 
    local fileName="$1";
    shift;
    substituteAll "$fileName" "$fileName" "$@"
}
substituteAllStream ()
{
 
    local -a args=();
    _allFlags;
    substituteStream "$1" "$2" "${args[@]}"
}
substituteInPlace ()
{
 
    local -a fileNames=();
    for arg in "$@";
    do
        if [[ "$arg" = "--"* ]]; then
            break;
        fi;
        fileNames+=("$arg");
        shift;
    done;
    if ! [[ "${#fileNames[@]}" -gt 0 ]]; then
        echo "substituteInPlace called without any files to operate on (files must come before options!)" 1>&2;
        return 1;
    fi;
    for file in "${fileNames[@]}";
    do
        substitute "$file" "$file" "$@";
    done
}
substituteStream ()
{
 
    local var=$1;
    local description=$2;
    shift 2;
    while (( "$#" )); do
        local replace_mode="$1";
        case "$1" in 
            --replace)
                if ! "$_substituteStream_has_warned_replace_deprecation"; then
                    echo "substituteStream() in derivation $name: WARNING: '--replace' is deprecated, use --replace-{fail,warn,quiet}. ($description)" 1>&2;
                    _substituteStream_has_warned_replace_deprecation=true;
                fi;
                replace_mode='--replace-warn'
            ;&
            --replace-quiet | --replace-warn | --replace-fail)
                pattern="$2";
                replacement="$3";
                shift 3;
                if ! [[ "${!var}" == *"$pattern"* ]]; then
                    if [ "$replace_mode" == --replace-warn ]; then
                        printf "substituteStream() in derivation $name: WARNING: pattern %q doesn't match anything in %s\n" "$pattern" "$description" 1>&2;
                    else
                        if [ "$replace_mode" == --replace-fail ]; then
                            printf "substituteStream() in derivation $name: ERROR: pattern %q doesn't match anything in %s\n" "$pattern" "$description" 1>&2;
                            return 1;
                        fi;
                    fi;
                fi;
                eval "$var"'=${'"$var"'//"$pattern"/"$replacement"}'
            ;;
            --subst-var)
                local varName="$2";
                shift 2;
                if ! [[ "$varName" =~ ^[a-zA-Z_][a-zA-Z0-9_]*$ ]]; then
                    echo "substituteStream() in derivation $name: ERROR: substitution variables must be valid Bash names, \"$varName\" isn't." 1>&2;
                    return 1;
                fi;
                if [ -z ${!varName+x} ]; then
                    echo "substituteStream() in derivation $name: ERROR: variable \$$varName is unset" 1>&2;
                    return 1;
                fi;
                pattern="@$varName@";
                replacement="${!varName}";
                eval "$var"'=${'"$var"'//"$pattern"/"$replacement"}'
            ;;
            --subst-var-by)
                pattern="@$2@";
                replacement="$3";
                eval "$var"'=${'"$var"'//"$pattern"/"$replacement"}';
                shift 3
            ;;
            *)
                echo "substituteStream() in derivation $name: ERROR: Invalid command line argument: $1" 1>&2;
                return 1
            ;;
        esac;
    done;
    printf "%s" "${!var}"
}
unpackFile ()
{
 
    curSrc="$1";
    echo "unpacking source archive $curSrc";
    if ! runOneHook unpackCmd "$curSrc"; then
        echo "do not know how to unpack source archive $curSrc";
        exit 1;
    fi
}
unpackPhase ()
{
 
    runHook preUnpack;
    if [ -z "${srcs:-}" ]; then
        if [ -z "${src:-}" ]; then
            echo 'variable $src or $srcs should point to the source';
            exit 1;
        fi;
        srcs="$src";
    fi;
    local -a srcsArray;
    concatTo srcsArray srcs;
    local dirsBefore="";
    for i in *;
    do
        if [ -d "$i" ]; then
            dirsBefore="$dirsBefore $i ";
        fi;
    done;
    for i in "${srcsArray[@]}";
    do
        unpackFile "$i";
    done;
    : "${sourceRoot=}";
    if [ -n "${setSourceRoot:-}" ]; then
        runOneHook setSourceRoot;
    else
        if [ -z "$sourceRoot" ]; then
            for i in *;
            do
                if [ -d "$i" ]; then
                    case $dirsBefore in 
                        *\ $i\ *)

                        ;;
                        *)
                            if [ -n "$sourceRoot" ]; then
                                echo "unpacker produced multiple directories";
                                exit 1;
                            fi;
                            sourceRoot="$i"
                        ;;
                    esac;
                fi;
            done;
        fi;
    fi;
    if [ -z "$sourceRoot" ]; then
        echo "unpacker appears to have produced no directories";
        exit 1;
    fi;
    echo "source root is $sourceRoot";
    if [ "${dontMakeSourcesWritable:-0}" != 1 ]; then
        chmod -R u+w -- "$sourceRoot";
    fi;
    runHook postUnpack
}
updateAutotoolsGnuConfigScriptsPhase ()
{
 
    if [ -n "${dontUpdateAutotoolsGnuConfigScripts-}" ]; then
        return;
    fi;
    for script in config.sub config.guess;
    do
        for f in $(find . -type f -name "$script");
        do
            echo "Updating Autotools / GNU config script to a newer upstream version: $f";
            cp -f "/nix/store/axmfg44mziw9bngfj937li7c7l39x6w1-gnu-config-2024-01-01/$script" "$f";
        done;
    done
}
updateSourceDateEpoch ()
{
 
    local path="$1";
    [[ $path == -* ]] && path="./$path";
    local -a res=($(find "$path" -type f -not -newer "$NIX_BUILD_TOP/.." -printf '%T@ "%p"\0' | sort -n --zero-terminated | tail -n1 --zero-terminated | head -c -1));
    local time="${res[0]//\.[0-9]*/}";
    local newestFile="${res[1]}";
    if [ "${time:-0}" -gt "$SOURCE_DATE_EPOCH" ]; then
        echo "setting SOURCE_DATE_EPOCH to timestamp $time of file $newestFile";
        export SOURCE_DATE_EPOCH="$time";
        local now="$(date +%s)";
        if [ "$time" -gt $((now - 60)) ]; then
            echo "warning: file $newestFile may be generated; SOURCE_DATE_EPOCH may be non-deterministic";
        fi;
    fi
}
PATH="$PATH${nix_saved_PATH:+:$nix_saved_PATH}"
XDG_DATA_DIRS="$XDG_DATA_DIRS${nix_saved_XDG_DATA_DIRS:+:$nix_saved_XDG_DATA_DIRS}"
export NIX_BUILD_TOP="$(mktemp -d -t nix-shell.XXXXXX)"
export TMP="$NIX_BUILD_TOP"
export TMPDIR="$NIX_BUILD_TOP"
export TEMP="$NIX_BUILD_TOP"
export TEMPDIR="$NIX_BUILD_TOP"
eval "${shellHook:-}"

