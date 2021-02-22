using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RayTracerNet
{
    public enum LogType
    {
        Info,
        Warnning,
        Error,
        Complete,
        Debugging,
    }

    public class LogItem
    {
        public LogType type;
        public string message;

        public LogItem(LogType type, string message)
        {
            this.type = type;
            this.message = message;
        }

        public Color GetFontColor()
        {
            switch (type)
            {
                case LogType.Error:
                    return Color.Color32(128,0,0);
                case LogType.Complete:
                    return Color.Color32(0,0,205);
                case LogType.Info:
                case LogType.Warnning:
                case LogType.Debugging:
                default:
                    return Color.Color32(0, 0, 0);
            }
        }
        public Color GetBackgroundColor()
        {
            switch (type)
            {
                case LogType.Error:
                    return Color.Color32(255, 140, 0); 
                case LogType.Warnning:
                    return Color.Color32(255, 255, 0);
                case LogType.Complete:
                    return Color.Color32(0, 128, 0);
                case LogType.Debugging:
                    return Color.Color32(0, 255, 255);
                case LogType.Info:
                default:
                    return Color.Color32(255, 255, 255);
            }
        }
    }

    public class Log
    {
        private static Log sInstance;

        private List<LogItem> m_logItems;

        private Log()
        {
            m_logItems = new List<LogItem>();
        }

        private static Log GetInstance()
        {
            if (sInstance == null)
                sInstance = new Log();
            return sInstance;
        }

        public static void Info(string message)
        {
            GetInstance().AddLogItem(LogType.Info, message);
        }

        public static void Warn(string message)
        {
            GetInstance().AddLogItem(LogType.Warnning, message);
        }

        public static void Err(string message)
        {
            GetInstance().AddLogItem(LogType.Error, message);
        }

        public static void CompleteInfo(string message)
        {
            GetInstance().AddLogItem(LogType.Complete, message);
        }

        public static void AddLog(LogType type, string message)
        {
            GetInstance().AddLogItem(type, message);
        }

        public static void Clear()
        {
            GetInstance().m_logItems.Clear();
            MessageHandler.Broadcast(MessageName.ClearLogs);
        }

        private void AddLogItem(LogType type, string message)
        {
            LogItem item = new LogItem(type, message);
            this.m_logItems.Add(item);
            MessageHandler.Broadcast<LogItem>(MessageName.AddLog, item);
        }
    }
}