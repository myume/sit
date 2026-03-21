{
  mkShell,
  clangStdenv,
  clang-tools,
  cmake,
  codespell,
  ninja,
}:
mkShell.override
{
  stdenv = clangStdenv;
} {
  packages = [
    clang-tools
    cmake
    codespell
    ninja
  ];
}
