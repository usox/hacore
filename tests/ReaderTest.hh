<?hh // partial
namespace Usox\Hacore;

use HackPack\HackUnit\Contract\Assert;

class ReaderTest {

	<<Test>>
	public function loadThrowsExceptionIfFileDoesNotExist(Assert $assert): void {
		$filename = 'some-file-that-does-not-exist';

		$reader = new Reader();

		$assert->whenCalled(() ==> {
			$reader->load($filename);
		})
		->willThrowClassWithMessage(
			Exception\ConfigFileNotFoundException::class,
			sprintf('File \'%s\' not found', $filename)
		);
	}

	<<Test>>
	public function loadThrowsExceptionIfFileContainsInvalidJson(Assert $assert): void {
		$filename = 'tests/config_file_with_invalid_json';

		$reader = new Reader();

		$assert->whenCalled(() ==> {
			$reader->load($filename);
		})
		->willThrowClassWithMessage(
			Exception\ConfigLoadingException::class,
			'Config loading failed: Syntax error'
		);
	}

	<<Test>>
	public function getLeafThrowsExceptionIfConfigIsNotLoaded(Assert $assert): void {
		$filename = 'tests/config_file_with_valid_json';
		$key = 'foo-bar';

		$reader = new Reader();

		$assert->whenCalled(() ==> {
			$reader->getLeaf($key);
		})
		->willThrowClass(
			Exception\ConfigNotLoadedException::class
		);
	}

	<<Test>>
	public function getLeafThrowsExceptionIfKeyDoesNotExist(Assert $assert): void {
		$filename = 'tests/config_file_with_valid_json';
		$key = 'foo-bar';

		$reader = new Reader();
		$reader->load($filename);

		$assert->whenCalled(() ==> {
			$reader->getLeaf($key);
		})
		->willThrowClassWithMessage(
			Exception\LeafNotFoundException::class,
			sprintf('Key \'%s\' not found', $key)
		);
	}

	<<Test>>
	public function getLeafThrowsExceptionIfLeafIsBranch(Assert $assert): void {
		$filename = 'tests/config_file_with_valid_json';
		$key = 'zomg';

		$reader = new Reader();
		$reader->load($filename);

		$assert->whenCalled(() ==> {
			$reader->getLeaf($key);
		})
		->willThrowClassWithMessage(
			Exception\LeafIsBranchException::class,
			sprintf('Expected \'%s\' to be a leaf but got a branch', $key)
		);
	}

	<<Test>>
	public function getLeafThrowsExceptionIfBranchIsLeaf(Assert $assert): void {
		$filename = 'tests/config_file_with_valid_json';
		$key = 'aggi';

		$reader = new Reader();
		$reader->load($filename);

		$assert->whenCalled(() ==> {
			$reader->getBranch($key);
		})
		->willThrowClassWithMessage(
			Exception\BranchIsLeafException::class,
			sprintf('Expected \'%s\' to be a branch but got a leaf', $key)
		);
	}

	<<Test>>
	public function getLeafReturnsLeafAsString(Assert $assert): void {
		$filename = 'tests/config_file_with_valid_json';

		$reader = new Reader();
		$reader->load($filename);

		$assert->string($reader->getLeaf('aggi'))->is('666');
	}

	<<Test>>
	public function getBranchReturnsBranchAsReaderInstanceWithConfig(Assert $assert): void {
		$filename = 'tests/config_file_with_valid_json';

		$reader = new Reader();
		$reader->load($filename);

		$branch = $reader->getBranch('zomg');

		$assert->string($branch->getLeaf('dr'))->is('brettermeier');
	}
}
