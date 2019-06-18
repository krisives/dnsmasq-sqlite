
# dnsmasq-sqlite

This is a slightly modified version of `dnsmasq` that requires that any domain
that will be resolved must first be in an Sqlite database, which may contain
lots of domains, potentially tens of millions to hundreds of millions of
domains.

## Building

Building from source on Ubuntu/Debian can be done with:

    sudo apt install build-essential libsqlite3-dev
    git clone https://github.com/krisives/dnsmasq-sqlite.git
    cd dnsmasq-sqlite
    make

You can now run a debugging version like this:

    ./src/dnsmasq -d -p 9999 --db-file domains.db --log-queries

With the DNS server running you can issue a test query via:

    dig @127.0.0.1 -p 9999 google.com

## Creating a Database

A basic script that downloads a list of the top 10 million domains and
imports it into an Sqlite3 database is provided in `createdb.sh`