{ pkgs ? import <nixpkgs> {} }: pkgs.mkShell {
	buildInputs = with pkgs; [
		gcc
		cmake
		ninja
		clang-tools
		lldb

		zlib
		curl
		nlohmann_json
	];
}

