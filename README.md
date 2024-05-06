### Pancake Demo

A very basic gltf scene viewer using the [pancake engine](https://github.com/tonkev/pancake-engine/).

![Demo](demo.gif)

**Instructions:**
```
  git clone https://github.com/tonkev/pancake-demo/ && cd pancake-demo
  mkdir build && cd build
  cmake ..
  make
  ./demo/demo --resource-paths ";../demo;../pancake/pancake" --gltf-scene <path-to-gltf-file>
```