{
  mkShell,
  clangStdenv,
  clang-tools,
  cmake,
  codespell,
}:
mkShell.override
{
  stdenv = clangStdenv;
} {
  packages = [
    clang-tools
    cmake
    codespell
  ];
}
