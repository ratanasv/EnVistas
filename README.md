EnVistas
========

A temp repo for converting [VISTAS](https://bitbucket.org/nikmolnar/vistas/overview) into a backend for [ENVISION](http://envision.bioe.orst.edu/Default.aspx).


###Build
Download the dependencies (target.zip) at https://bitbucket.org/nikmolnar/vistas/downloads. You need to define a few environment variables:

-VISTAS_DIR, this is the directory containing the unzipped dependencies.

-BOOST_DIR, self-explanatory

The current build will copy all .dll files from the dependencies folder into the executable directory.

Your .envx file then needs the following configuration:
```
<visualizers>
  <visualizer
    name = 'EnVistas'
    path = 'OUTPUT_PATH\EnVistas.dll'
    id = '0'
    use = '1'
    type = '6'
    initInfo = ''
  />
</visualizers>
```
`OUTPUT_PATH` is simply the output directory of your Envision build.