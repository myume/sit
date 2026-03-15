{
  stdenv,
  lib,
}:
stdenv.mkDerivation {
  pname = "package";
  version = "1.2.3";
  src = ../.;
  buildInputs = [
  ];

  meta = {
    description = "A program that does something";
    longDescription = ''
      Let's hope this program continues to do that something,
      forever.
    '';
    homepage = "https://example.com";
    license = lib.licenses.mit;
    maintainers = with lib.maintainers; [you];
    platforms = lib.platforms.all;
  };
}
