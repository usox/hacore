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

use function Facebook\FBExpect\expect;
use namespace HH\Lib\Str;

class ReaderTest extends \Facebook\HackTest\HackTest {

	public function testLoadThrowsExceptionIfFileDoesNotExist(): void {
		$filename = 'some-file-that-does-not-exist';

		$reader = new Reader();

		expect(() ==> $reader->load($filename))
		->toThrow(
			Exception\ConfigFileNotFoundException::class,
			Str\format('File \'%s\' not found', $filename)
		);
	}

	public function testLoadThrowsExceptionIfFileContainsInvalidJson(): void {
		$filename = 'tests/config_file_with_invalid_json';

		$reader = new Reader();

		expect(() ==> $reader->load($filename))
		->toThrow(
			Exception\ConfigLoadingException::class,
			'Config loading failed: Syntax error'
		);
	}

	public function testGetLeafThrowsExceptionIfConfigIsNotLoaded(): void {
		$reader = new Reader();

		expect(() ==> $reader->getLeaf('foo-bar'))
		->toThrow(
			Exception\ConfigNotLoadedException::class
		);
	}

	public function testGetLeafThrowsExceptionIfKeyDoesNotExist(): void {
		$filename = 'tests/config_file_with_valid_json';
		$key = 'foo-bar';

		$reader = new Reader();
		$reader->load($filename);

		expect(() ==> $reader->getLeaf($key))
		->toThrow(
			Exception\LeafNotFoundException::class,
			Str\format('Key \'%s\' not found', $key)
		);
	}

	public function testGetLeafThrowsExceptionIfLeafIsBranch(): void {
		$filename = 'tests/config_file_with_valid_json';
		$key = 'zomg';

		$reader = new Reader();
		$reader->load($filename);

		expect(() ==> $reader->getLeaf($key))
		->toThrow(
			Exception\LeafIsBranchException::class,
			Str\format('Expected \'%s\' to be a leaf but got a branch', $key)
		);
	}

	public function testGetLeafThrowsExceptionIfBranchIsLeaf(): void {
		$filename = 'tests/config_file_with_valid_json';
		$key = 'aggi';

		$reader = new Reader();
		$reader->load($filename);

		expect(() ==> $reader->getBranch($key))
		->toThrow(
			Exception\BranchIsLeafException::class,
			Str\format('Expected \'%s\' to be a branch but got a leaf', $key)
		);
	}

	public function testGetLeafReturnsLeafAsString(): void {
		$filename = 'tests/config_file_with_valid_json';

		$reader = new Reader();
		$reader->load($filename);

		expect($reader->getLeaf('aggi'))->toBeSame('666');
	}

	public function testGetBranchReturnsBranchAsReaderInstanceWithConfig(): void {
		$filename = 'tests/config_file_with_valid_json';

		$reader = new Reader();
		$reader->load($filename);

		$branch = $reader->getBranch('zomg');

		expect($branch->getLeaf('dr'))->toBeSame('brettermeier');
	}
}
