#!/bin/bash

make collect_cents || exit 1

./collect_cents 362294.cgm.epos.m.v3
