# SPDX-FileCopyrightText: Copyright (C) 2024-2025 Marek Küthe <m.k@mk16.de>
#
# SPDX-License-Identifier: GPL-3.0-or-later

.PHONY: all setup addresssanitizer leaksanitizer undefinedsanitizer clean compile coverage install debian check cppcheck flawfinder selene luacheck mdl lizard clangtidy reuse-annotate reuse-download reuse-lint reuse-fix reuse test clangformat

all: setup compile

setup:
	test -d build || meson setup build

sanitizer:
	meson setup --reconfigure --debug -Db_sanitize=address,undefined build

native:
	meson setup --reconfigure -Dnative=true build

setupcoverage:
	meson setup --reconfigure -Db_coverage=true build

clean: setup
	meson compile --clean -C build

compile: setup
	meson compile -C build

coverage: setupcoverage test
	ninja -C build coverage

install: setup compile
	meson install -C build

debian:
	dpkg-buildpackage -b

check: flawfinder cppcheck scanbuild clangtidy lizard reuse mdl selene luacheck

cppcheck: setup
	meson compile -C build cppcheck

infer: setup
	meson compile -C build infer

flawfinder: setup
	meson compile -C build flawfinder

selene: setup
	meson compile -C build selene

luacheck: setup
	meson compile -C build luacheck

lizard: setup
	meson compile -C build lizard

# || true is a workaround for
# https://github.com/boostorg/boost/issues/1107
clangtidy: setup
	ninja -C build clang-tidy || true

# || true is a workaround for
# https://github.com/boostorg/boost/issues/1107
scanbuild: setup
	ninja -C build scan-build || true

mdl: setup
	meson compile -C build mdl

reuse-annotate: setup
	meson compile -C build reuse-annotate

reuse-download: setup
	meson compile -C build reuse-download

reuse-lint: setup
	meson compile -C build reuse-lint

reuse-fix: reuse-annotate reuse-download

reuse: reuse-lint

test: setup
	meson test -C build

clangformat: setup
	ninja -C build clang-format
