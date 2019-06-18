#!/usr/bin/env bash

url="https://www.domcop.com/files/top/top10milliondomains.csv.zip"

if [ ! -f "top10milliondomains.csv" ]; then
  if [ ! -f "domains.zip" ]; then
    curl -o "domains.zip" "$url"
  fi
  
  unzip "domains.zip"
fi

sqlite3 <<STDIN domains.db
.mode csv
.import top10milliondomains.csv domain
create unique index idx_Domain on domain(Domain)
STDIN