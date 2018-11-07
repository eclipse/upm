# UPM Website [![Build Status](https://travis-ci.org/intel-iot-devkit/upm-site.svg?branch=master)](https://travis-ci.org/intel-iot-devkit/upm-site)

## GitHub Pages URL: https://upm.mraa.io
## Dependencies:
* [NodeJs](https://nodejs.org/en/)
* [Gulp](https://github.com/gulpjs/gulp)
* [Python](https://www.python.org/), specifically Python 2!
* [Ruby](https://www.ruby-lang.org/en/)
* [Sass](http://sass-lang.com/)
* [Bundler](http://bundler.io/)
* A bunch of other stuff installed through bundle and ruby gems

## Prepare Dev Environment:
* Run `npm install`, in the same directory as the code. This will install all the  dependencies automatically
* Run `bundler install` to install Gems dependencies automatically

## Gulp Tasks:
1. `gulp clean`: to clean local build directories and resources
2. `gulp build`: to generate the build for production using _config.yml, deploys to /_site
3. `gulp build:local`: to generated the build using _config_dev.yml
4. `gulp serve`: runs the app locally, using the _config_dev.yml file

## Global.yml
* Urls defined for fetching images, source code, and apis
* Whenever there are changes in facets.json/sensordetail.json files, increment the value in ajaxVersion to refresh the cache
* Whenever there are changes in css/js resources, increment the value in resourceVersion to refresh the cache

## _assets/gulp_config/paths.js
* Set paths.sensorDataSrc to the root of UPM /src

## Static Content:
* The sensor data is kept under /_assets/content/sensorDetail.json
  To generate this file with `jq` and latest `upm` sources use:
  ```
  find upm/src/ -iname "*.json" | sort | xargs jq -s -c '.' >sensorDetail.json
  ```
* The facets data is kept under /_assets/content/facets.json
* If images are missing, copy it over from _assets/gulp_config/images to _/site/assets/images

## Miscellaneous:
* _config_dev.yml has configurable properties for local
* _config.yml has properties for production deployment
* CNAME corresponds to the mapped cname in DNS provider

## Site Deployment using Travis CI

Documentation can be found [here](https://docs.travis-ci.com/user/deployment/pages/)
