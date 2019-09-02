// Licensed under the Apache License, Version 2.0 (the "License"); you may not
// use this file except in compliance with the License. You may obtain a copy of
// the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
// License for the specific language governing permissions and limitations under
// the License.
#pragma once

#include <list>
#include <unordered_map>

template <typename Key, typename Value>
class LRU {
public:
	explicit LRU(size_t capacity) : capacity(capacity), keyAge(capacity){};

	void add(Key key, Value val) {
		if (!hasKey(key)) {
			if (size() == capacity) {
				purge();
			}

			auto it = keyAge.insert(keyAge.begin(), key);
			items[key] = std::make_pair(val, it);
			return;
		}

		moveToFront(key);
	}

	Value get(Key key) {
		if (!hasKey(key)) {
			return Value();
		}

		auto iter = items.find(key);
		moveToFront(key);

		return iter->second.first;
	}

	size_t size() { return keyAge.size(); }

	bool hasKey(const Key& key) { return !(items.find(key) == items.end()); }

	void erase(const Key& key) {
		if (!hasKey(key)) {
			return;
		}

		auto pair = items.find(key);

		keyAge.erase(pair->second.second);
		items.erase(key);
	}

private:
	typedef std::list<Key> KeyListType;
	typedef std::pair<Value, typename KeyListType::iterator> KVPair;

	KeyListType keyAge;
	std::unordered_map<Key, KVPair> items;
	size_t capacity;

	void purge() {
		auto key = keyAge.back();
		items.erase(key);
		keyAge.pop_back();
	};

	void moveToFront(Key key) {
		auto pair = items[key];
		keyAge.splice(keyAge.begin(), keyAge, pair.second);
	}
};