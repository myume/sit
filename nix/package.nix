{
  stdenv,
  lib,
}:
stdenv.mkDerivation {
  pname = "sit";
  version = "1.2.3";
  src = ../.;
  buildInputs = [
  ];

  meta = {
    description = "Simple Image Transformer";
    homepage = "https://github.com/myume/sit";
    license = lib.licenses.mit;
    maintainers = with lib.maintainers; [you];
    platforms = lib.platforms.all;
  };
}
