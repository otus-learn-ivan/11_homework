#define BOOST_TEST_MODULE test_bulk

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <iterator>
#include <iomanip>

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string>

#include <sys/types.h>
#include <dirent.h>
#include <thread>
#include <chrono>

#include <boost/test/unit_test.hpp>
#include <boost/process.hpp>
#include <iostream>
#include <chrono>
#include <signal.h>

BOOST_AUTO_TEST_SUITE(test_bulk)

BOOST_AUTO_TEST_CASE(test_valid)
{
   std::cout<< "START TEST " << std::endl;
   pid_t pid;
   // Запуск join_server как отдельного процесса
   if ((pid = fork()) == 0) {
     // Код для join_server
     system("./join_server 9000");
     exit(0);
   }

   std::this_thread::sleep_for(std::chrono::milliseconds(50));

   std::string command("pgrep join_server");
   FILE* pipe = popen(command.c_str(), "r"); // Запускаем команду и получаем поток

   if (pipe != nullptr) {
     char buffer[128];
     if (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
       pid = std::stoi(buffer);
       std::cout << "PID процесса join_server: " << pid << std::endl;
     } else {
       std::cerr << "Ошибка при чтении из потока" << std::endl;
     }
    pclose(pipe);
  } else {
    std::cerr << "Ошибка при выполнении команды popen" << std::endl;
  }


  std::this_thread::sleep_for(std::chrono::milliseconds(50));

  system("echo \"INSERT A 0 lean\" | nc localhost 9000");
  system("echo \"INSERT A 0 understand\" | nc localhost 9000");
  system("echo \"INSERT A 1 sweater\" | nc localhost 9000");
  system("echo \"INSERT A 2 frank\" | nc localhost 9000");
  system("echo \"INSERT A 3 violation\" | nc localhost 9000");
  system("echo \"INSERT A 4 quality\" | nc localhost 9000");
  system("echo \"INSERT A 5 precision\" | nc localhost 9000");

  system("echo \"INSERT B 3 proposal\" | nc localhost 9000");
  system("echo \"INSERT B 4 example\" | nc localhost 9000");
  system("echo \"INSERT B 5 lake\" | nc localhost 9000");
  system("echo \"INSERT B 6 flour\" | nc localhost 9000");
  system("echo \"INSERT B 7 wonder\" | nc localhost 9000");
  system("echo \"INSERT B 8 selection\" | nc localhost 9000");

  system("echo \"INTERSECTION\" | nc localhost 9000");
  system("echo \"SYMMETRIC_DIFFERENCE\" | nc localhost 9000");
  system("echo \"INTERSECTION\" | nc localhost 9000");
  system("echo \"TRUNCATE A\" | nc localhost 9000");
  system("echo \"TRUNCATE B\" | nc localhost 9000");



  kill(pid, SIGINT);

  std::this_thread::sleep_for(std::chrono::milliseconds(2000));

  std::cout <<"Ожидание завершения join_server pid: " <<pid <<std::endl;
  waitpid(pid, nullptr, 0);  // Ожидание завершения join_server

}

BOOST_AUTO_TEST_SUITE_END()
