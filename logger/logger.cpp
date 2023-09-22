#include "logger.h"

enum class MessageType {
    Info,
    Warning,
    Error
};

struct logmsg
{
  std::chrono::system_clock::time_point timestamp;
  std::string sender;
  MessageType msg_type;
  std::string msg_data;
} ;


enum class LogMode
{
  FileOnly,
  TerminalOnly,
  FileAndTerminal
};

class Logger {
  
  public:

  Logger()
  {
    m_LoggerMode = LogMode::FileAndTerminal;
  }

  Logger(const LogMode& log_mode)
    : m_LoggerMode(log_mode)
  {

  }



  private:
    std::queue<logmsg> logQueue;
    std::ofstream outfile;


    LogMode m_LoggerMode;
  public:
    void log(const logmsg& message) {
        logQueue.push(message);

        
    }

    void processLog(int opt){
      
      switch(m_LoggerMode) {
        case LogMode::FileOnly :
          writeToFile();
          break;
        case LogMode::TerminalOnly :
          displayOnTerminal();
          break;
        case LogMode::FileAndTerminal :
          writeToFile();
          displayOnTerminal();
          break;
            
      }
    }

    private:
    std::string messageTypeToString(MessageType type) {
        switch (type) {
            case MessageType::Info:
                return "Info";
            case MessageType::Warning:
                return "Warning";
            case MessageType::Error:
                return "Error";
            default:
                return "Unknown"; 
        }
    }

  
  private:
    void writeToFile() {
    const auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    std::tm timeinfo = *std::localtime(&now_c);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", &timeinfo);

    std::string filename = std::string(buffer) + ".txt";

    static std::string previousFilename;  
    static bool firstLog = true;  

    if (filename != previousFilename || firstLog) {
        
        if (!firstLog) {
            
            outfile.close();
        }

        outfile.open(filename, std::ios::app);
        previousFilename = filename;
        firstLog = false;
    }

    while (!logQueue.empty()) {
        logmsg message = logQueue.front();
        logQueue.pop();

        auto timestamp = std::chrono::system_clock::to_time_t(message.timestamp);

        std::string msgTypeStr = messageTypeToString(message.msg_type);


        outfile << std::put_time(std::localtime(&timestamp), "%Y-%m-%d %H:%M:%S") << " | " << message.sender << " | " << msgTypeStr << " | " << message.msg_data << "\n";
    }

    outfile.close();  
    std::cout << "Log data written to file: " << filename << std::endl;
}


    void displayOnTerminal() {
        while (!logQueue.empty()) {
            logmsg message = logQueue.front();
            logQueue.pop();

            auto timestamp = std::chrono::system_clock::to_time_t(message.timestamp);

            std::string msgTypeStr = messageTypeToString(message.msg_type);

            std::cout << std::put_time(std::localtime(&timestamp), "%Y-%m-%d %H:%M:%S") << " | " << message.sender << " | " << msgTypeStr << " | " << message.msg_data << "\n";
        }
    }
};


int main(){
  Logger logger;
  int opt;

  do{
    std::cout << "Enter an option: (0: Write to file, 1: Display on terminal, 2: Both)" << std::endl;
    std::cin >> opt;

    if(opt != -1){
      logmsg message;
      message.timestamp= std::chrono::system_clock::now();
      std::cout <<"Enter sender:" << "\n"<< std::endl;
      std::cin >>message.sender;
      std::cout << "Enter message type (0: Info, 1: Warning, 2: Error): ";
      int msgType;
      std::cin >> msgType;
      message.msg_type = static_cast<MessageType>(msgType);
      std::cin.ignore();
      std::cout << "Enter message data" << "\n" << std::endl;
      std::getline(std::cin, message.msg_data);


      logger.log(message);
      logger.processLog(opt);
    }
      
  } while(opt != -1);

  return 0;
}
    


