<?hh // strict
namespace Usox\Hacore;

interface ReaderInterface {

	public function load(string $config_file_path): void;

	public function getLeaf(string $key): string;

	public function getBranch(string $key): ReaderInterface;
}
