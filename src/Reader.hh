<?hh // strict
namespace Usox\Hacore;

use HH\Lib\{Str, C};

final class Reader implements ReaderInterface {

	public function __construct(
		private ?dict<string, mixed> $config = null
	): void {
	}

	public function load(string $config_file_path): void {
		$real_file_path = \realpath($config_file_path);

		if (false === \file_exists($real_file_path)) {
			throw new Exception\ConfigFileNotFoundException(
				Str\format('File \'%s\' not found', $config_file_path)
			);
		}

		$file_content = \file_get_contents(
			$real_file_path
		);

		$decoded_config = \json_decode($file_content, true);

		if (\json_last_error() !== \JSON_ERROR_NONE) {
			throw new Exception\ConfigLoadingException(
				Str\format('Config loading failed: %s', \json_last_error_msg())
			);
		}

		$this->config = dict($decoded_config);
	}

	public function getLeaf(string $key): string {
		$value = $this->getValueByKey($key);

		if (is_array($value)) {
			throw new Exception\LeafIsBranchException(
				Str\format('Expected \'%s\' to be a leaf but got a branch', $key)
			);
		}
		return (string) $value;
	}

	public function getBranch(string $key): ReaderInterface {
		$value = $this->getValueByKey($key);

		if (!is_array($value)) {
			throw new Exception\BranchIsLeafException(
				Str\format('Expected \'%s\' to be a branch but got a leaf', $key)
			);
		}

		return new static(dict($value));
	}

	private function getValueByKey(string $key): mixed {
		$config = $this->config;

		if ($config === null) {
			throw new Exception\ConfigNotLoadedException();
		}
		if (!C\contains_key($config, $key)) {
			throw new Exception\LeafNotFoundException(
				Str\format('Key \'%s\' not found', $key)
			);
		}

		return $config[$key];
	}
}
