del /q /f "%S3E_DIR%\..\extensions\ouyaFace\*.*"
del /q /f "%S3E_DIR%\..\extensions\ouyaFace\h\*.*"
del /q /f "%S3E_DIR%\..\extensions\ouyaFace\lib\android\*.*"
del /q /f "%S3E_DIR%\..\extensions\ouyaFace\source\android\*.*"
del /q /f "%S3E_DIR%\..\extensions\ouyaFace\source\generic\*.*"
del /q /f "%S3E_DIR%\..\extensions\ouyaFace\source\h\*.*"
del /q /f "%S3E_DIR%\..\extensions\ouyaFace\stamp\*.*"

mkdir "%S3E_DIR%\..\extensions\ouyaFace"

mkdir "%S3E_DIR%\..\extensions\ouyaFace\h"

mkdir "%S3E_DIR%\..\extensions\ouyaFace\interface"

mkdir "%S3E_DIR%\..\extensions\ouyaFace\lib"

mkdir "%S3E_DIR%\..\extensions\ouyaFace\lib\android"

mkdir "%S3E_DIR%\..\extensions\ouyaFace\source"

mkdir "%S3E_DIR%\..\extensions\ouyaFace\source\h"

mkdir "%S3E_DIR%\..\extensions\ouyaFace\source\android"

mkdir "%S3E_DIR%\..\extensions\ouyaFace\source\generic"

mkdir "%S3E_DIR%\..\extensions\ouyaFace\stamp"

copy "ouyaFace.mkf" "%S3E_DIR%\..\extensions\ouyaFace\ouyaFace.mkf"

copy "ouyaFace.s4e" "%S3E_DIR%\..\extensions\ouyaFace\ouyaFace.s4e"

copy "ouyaFace_android.mkb" "%S3E_DIR%\..\extensions\ouyaFace\ouyaFace_android.mkb"

copy "ouyaFace_android_java.mkb" "%S3E_DIR%\..\extensions\ouyaFace\ouyaFace_android_javas.mkb"

copy "ouyaFace_build.mkf" "%S3E_DIR%\..\extensions\ouyaFace\ouyaFace_build.mkf"

copy "h\*" "%S3E_DIR%\..\extensions\ouyaFace\h\"

copy "interface\*" "%S3E_DIR%\..\extensions\ouyaFace\interface\"

copy "lib\android\*" "%S3E_DIR%\..\extensions\ouyaFace\lib\android\"

copy "source\*" "%S3E_DIR%\..\extensions\ouyaFace\source\h\"

copy "source\*" "%S3E_DIR%\..\extensions\ouyaFace\source\android\"

copy "source\*" "%S3E_DIR%\..\extensions\ouyaFace\source\generic\"

copy "stamp\*" "%S3E_DIR%\..\extensions\ouyaFace\stamp\"

echo Done.
pause