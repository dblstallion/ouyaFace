@echo off
mkdir "%S3E_DIR%\..\extensions\ouyaFace"

@echo off
mkdir "%S3E_DIR%\..\extensions\ouyaFace\h"

@echo off
mkdir "%S3E_DIR%\..\extensions\ouyaFace\interface"

@echo off
mkdir "%S3E_DIR%\..\extensions\ouyaFace\lib"

@echo off
mkdir "%S3E_DIR%\..\extensions\ouyaFace\lib\android"

@echo off
mkdir "%S3E_DIR%\..\extensions\ouyaFace\source"

@echo off
mkdir "%S3E_DIR%\..\extensions\ouyaFace\source\h"

@echo off
mkdir "%S3E_DIR%\..\extensions\ouyaFace\source\android"

@echo off
mkdir "%S3E_DIR%\..\extensions\ouyaFace\source\generic"

@echo off
mkdir "%S3E_DIR%\..\extensions\ouyaFace\stamp"

@echo off
copy "ouyaFace.mkf" "%S3E_DIR%\..\extensions\ouyaFace\ouyaFace.mkf"

@echo off
copy "ouyaFace.s4e" "%S3E_DIR%\..\extensions\ouyaFace\ouyaFace.s4e"

@echo off
copy "ouyaFace_android.mkb" "%S3E_DIR%\..\extensions\ouyaFace\ouyaFace_android.mkb"

@echo off
copy "ouyaFace_android_java.mkb" "%S3E_DIR%\..\extensions\ouyaFace\ouyaFace_android_javas.mkb"

@echo off
copy "ouyaFace_build.mkf" "%S3E_DIR%\..\extensions\ouyaFace\ouyaFace_build.mkf"

@echo off
copy "h\*" "%S3E_DIR%\..\extensions\ouyaFace\h\"

@echo off
copy "interface\*" "%S3E_DIR%\..\extensions\ouyaFace\interface\"

@echo off
copy "lib\android\*" "%S3E_DIR%\..\extensions\ouyaFace\lib\android\"

@echo off
copy "source\*" "%S3E_DIR%\..\extensions\ouyaFace\source\h\"

@echo off
copy "source\*" "%S3E_DIR%\..\extensions\ouyaFace\source\android\"

@echo off
copy "source\*" "%S3E_DIR%\..\extensions\ouyaFace\source\generic\"

@echo off
copy "stamp\*" "%S3E_DIR%\..\extensions\ouyaFace\stamp\"

@echo Done.
pause