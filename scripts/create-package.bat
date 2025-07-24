@SETLOCAL

@SET "build_mode=Debug"

@IF NOT "%~1"=="" SET "build_mode=%~1"

pipenv run -- ^
    conan create ^
        -pr:a default ^
        -s:a build_type=%build_mode% ^
        --build missing ^
        .

@ENDLOCAL
