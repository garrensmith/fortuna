# Licensed under the Apache License, Version 2.0 (the "License"); you may not
# use this file except in compliance with the License. You may obtain a copy of
# the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
# License for the specific language governing permissions and limitations under
# the License.

.PHONY: init build clean format clean-deps

all: build

init:
	test -d build || (mkdir build && cmake -S . -B build)

build: init
	make -C build

clean:
	rm -rf build
	rm -rf priv

clean-deps: clean
	rm -rf deps/v8
	rm -rf deps/.cipd
	rm -rf deps/.gclient
	rm -rf deps/.gclient_entries
	rm -rf deps/depot_tools
	


test: init
	make tests -C build
	build/bin/tests

format: 
	clang-format -style=file -i jsenv/*.cpp jsenv/*.h tests/*.cpp c_src/*.cpp c_src/*.h
