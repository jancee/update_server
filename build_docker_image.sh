#!/bin/bash

docker build . -t jancee/update-server:latest

docker tag jancee/update-server:latest jancee/seeed-update-image-generator:1.3.2
docker tag jancee/update-server:latest jancee/seeed-update-image-generator:latest
docker tag jancee/update-server:latest seeed.azurecr.io/seeed-update-image-generator:1.3.2
docker tag jancee/update-server:latest seeed.azurecr.io/seeed-update-image-generator:latest

docker push jancee/seeed-update-image-generator:1.3.2
docker push jancee/seeed-update-image-generator:latest
docker push seeed.azurecr.io/seeed-update-image-generator:1.3.2
docker push seeed.azurecr.io/seeed-update-image-generator:latest

