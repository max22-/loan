#ifndef CONFIG_H
#define CONFIG_H


class Config
{
public:
    static Config& getInstance();
    Config(Config&) = delete;
    Config& operator=(Config&) = delete;

    int maxRecordingTimeS(void);

private:
    Config();
    ~Config();
};

#endif // CONFIG_H
