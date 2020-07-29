# Software_defined_storage_DBsystem
file management system using C++

* 일정 크기 이상의 file은 filesystem에, 그 이하 크기의 file은 database에 저장
* 해당 file의 접근 횟수에 따라 hot/cold file로 분류
* data에 대한 data, metadata 제공
* 함수를 선언한 header file과 함수를 구체화한 header_materialization.cpp
