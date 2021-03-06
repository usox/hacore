#!/usr/bin/env hhvm
/*
 *  Copyright (c) 2019-present, Daniel Jakob
 *  All rights reserved.
 *
 *  This source code is licensed under the MIT license found in the
 *  LICENSE file in the root directory of this source tree.
 *
 */

namespace Usox\Hacore;

interface ReaderInterface {

	public function load(string $config_file_path): void;

	public function getLeaf(string $key): string;

	public function getBranch(string $key): ReaderInterface;
}
