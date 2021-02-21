using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RayTracerNet
{
    public delegate void MessageFunc();
    public delegate void MessageFunc<T>(T arg);
    public delegate void MessageFunc<T1, T2>(T1 arg1, T2 arg2);
    public delegate void MessageFunc<T1, T2, T3>(T1 arg1, T2 arg2, T3 arg3);
    public delegate void MessageFunc<T1, T2, T3, T4>(T1 arg1, T2 arg2, T3 arg3, T4 arg4);

    public enum MessageName
    {
        AddNewSceneObject,
        AddNewResource,

        RemoveSceneObject,
        RemoveResource,

        ClearLogs,
        AddLog,

        ChangeIntegratorType,

        SelectSceneObject,
    }

    public class MessageHandler
    {
        private readonly Dictionary<MessageName, Delegate> m_messageTable = new Dictionary<MessageName, Delegate>(MessageNameEnumComparer.Instance);

        private static MessageHandler sInstance;

        private static MessageHandler GetInstance()
        {
            if (sInstance == null)
                sInstance = new MessageHandler();
            return sInstance;
        }

        public static void AddListener(MessageName message, MessageFunc handler)
        {
            MessageHandler instance = GetInstance();
            if (instance.PreListenerAdding(message, handler))
            {
                instance.m_messageTable[message] = (MessageFunc)Delegate.Combine((MessageFunc)instance.m_messageTable[message], handler);
            }
        }

        public static void AddListener<T>(MessageName message, MessageFunc<T> handler)
        {
            MessageHandler instance = GetInstance();
            if (instance.PreListenerAdding(message, handler))
            {
                instance.m_messageTable[message] = (MessageFunc<T>)Delegate.Combine((MessageFunc<T>)instance.m_messageTable[message], handler);
            }
        }

        public static void AddListener<T1, T2>(MessageName message, MessageFunc<T1, T2> handler)
        {
            MessageHandler instance = GetInstance();
            if (instance.PreListenerAdding(message, handler))
            {
                instance.m_messageTable[message] = (MessageFunc<T1, T2>)Delegate.Combine((MessageFunc<T1, T2>)instance.m_messageTable[message], handler);
            }
        }

        public static void AddListener<T1, T2, T3>(MessageName message, MessageFunc<T1, T2, T3> handler)
        {
            MessageHandler instance = GetInstance();
            if (instance.PreListenerAdding(message, handler))
            {
                instance.m_messageTable[message] = (MessageFunc<T1, T2, T3>)Delegate.Combine((MessageFunc<T1, T2, T3>)instance.m_messageTable[message], handler);
            }
        }

        public static void AddListener<T1, T2, T3, T4>(MessageName message, MessageFunc<T1, T2, T3, T4> handler)
        {
            MessageHandler instance = GetInstance();
            if (instance.PreListenerAdding(message, handler))
            {
                instance.m_messageTable[message] = (MessageFunc<T1, T2, T3, T4>)Delegate.Combine((MessageFunc<T1, T2, T3, T4>)instance.m_messageTable[message], handler);
            }
        }

        public static void Broadcast(MessageName message)
        {
            MessageHandler instance = GetInstance();
            Delegate _delegate;
            if (instance.PreBroadcasting(message, out _delegate))
            {
                if (_delegate == null) return;
                MessageFunc source = instance.m_messageTable[message] as MessageFunc;
                if (source != null)
                {
                    source();
                }
            }
        }

        public static void Broadcast<T>(MessageName message, T arg)
        {
            MessageHandler instance = GetInstance();
            Delegate _delegate;
            if (instance.PreBroadcasting(message, out _delegate))
            {
                if (_delegate == null) return;
                MessageFunc<T> source = instance.m_messageTable[message] as MessageFunc<T>;
                if (source != null)
                {
                    source(arg);
                }
            }
        }

        public void Broadcast<T1, T2>(MessageName message, T1 arg1, T2 arg2)
        {
            MessageHandler instance = GetInstance();
            Delegate _delegate;
            if (instance.PreBroadcasting(message, out _delegate))
            {
                if (_delegate == null) return;
                MessageFunc<T1, T2> source = instance.m_messageTable[message] as MessageFunc<T1, T2>;
                if (source != null)
                {
                    source(arg1, arg2);
                }

            }
        }

        public void Broadcast<T1, T2, T3>(MessageName message, T1 arg1, T2 arg2, T3 arg3)
        {
            MessageHandler instance = GetInstance();
            Delegate _delegate;
            if (instance.PreBroadcasting(message, out _delegate))
            {
                if (_delegate == null) return;
                MessageFunc<T1, T2, T3> source = instance.m_messageTable[message] as MessageFunc<T1, T2, T3>;
                if (source != null)
                {
                    source(arg1, arg2, arg3);
                }
            }
        }

        public void Broadcast<T1, T2, T3, T4>(MessageName message, T1 arg1, T2 arg2, T3 arg3, T4 arg4)
        {
            MessageHandler instance = GetInstance();
            Delegate _delegate;
            if (instance.PreBroadcasting(message, out _delegate))
            {
                if (_delegate == null) return;
                MessageFunc<T1, T2, T3, T4> source = instance.m_messageTable[message] as MessageFunc<T1, T2, T3, T4>;
                if (source != null)
                {
                    source(arg1, arg2, arg3, arg4);
                }
            }
        }

        private void PostListenerRemoving(MessageName message)
        {
            if (m_messageTable.ContainsKey(message) && (m_messageTable[message] == null))
            {
                m_messageTable.Remove(message);
            }
        }

        private bool PreBroadcasting(MessageName message, out Delegate _delegate)
        {
            if (!m_messageTable.ContainsKey(message))
            {
                _delegate = null;
                return false;
            }
            else
            {
                _delegate = m_messageTable[message];
                return true;
            }
        }

        private bool PreListenerAdding(MessageName message, Delegate listenerForAdding)
        {
            bool flag = true;
            if (!m_messageTable.ContainsKey(message))
            {
                m_messageTable.Add(message, null);
            }
            Delegate delegate2 = m_messageTable[message];
            if ((delegate2 != null) && (delegate2.GetType() != listenerForAdding.GetType()))
            {
                flag = false;
            }
            return flag;
        }

        private bool PreListenerRemoving(MessageName message, Delegate listenerForRemoving)
        {
            bool flag = true;
            if (m_messageTable.ContainsKey(message))
            {
                Delegate delegate2 = m_messageTable[message];
                if (delegate2 == null)
                {
                    flag = false;
                    return flag;
                }
                if (delegate2.GetType() != listenerForRemoving.GetType())
                {
                    flag = false;
                }
                return flag;
            }
            flag = false;
            return flag;
        }

        public void RemoveListener(MessageName message, MessageFunc handler)
        {
            if (PreListenerRemoving(message, handler))
            {
                m_messageTable[message] = (MessageFunc)Delegate.Remove((MessageFunc)m_messageTable[message], handler);
            }
            PostListenerRemoving(message);
        }

        public void RemoveListener<T>(MessageName message, MessageFunc<T> handler)
        {
            if (PreListenerRemoving(message, handler))
            {
                m_messageTable[message] = (MessageFunc<T>)Delegate.Remove((MessageFunc<T>)m_messageTable[message], handler);
            }
            PostListenerRemoving(message);
        }

        public void RemoveListener<T1, T2>(MessageName message, MessageFunc<T1, T2> handler)
        {
            if (PreListenerRemoving(message, handler))
            {
                m_messageTable[message] = (MessageFunc<T1, T2>)Delegate.Remove((MessageFunc<T1, T2>)m_messageTable[message], handler);
            }
            PostListenerRemoving(message);
        }

        public void RemoveListener<T1, T2, T3>(MessageName message, MessageFunc<T1, T2, T3> handler)
        {
            if (PreListenerRemoving(message, handler))
            {
                m_messageTable[message] = (MessageFunc<T1, T2, T3>)Delegate.Remove((MessageFunc<T1, T2, T3>)m_messageTable[message], handler);
            }
            PostListenerRemoving(message);
        }

        public void RemoveListener<T1, T2, T3, T4>(MessageName message, MessageFunc<T1, T2, T3, T4> handler)
        {
            if (PreListenerRemoving(message, handler))
            {
                m_messageTable[message] = (MessageFunc<T1, T2, T3, T4>)Delegate.Remove((MessageFunc<T1, T2, T3, T4>)m_messageTable[message], handler);
            }
            PostListenerRemoving(message);
        }
    }

    class MessageNameEnumComparer : IEqualityComparer<MessageName>
    {
        public static readonly MessageNameEnumComparer Instance = new MessageNameEnumComparer();

        public bool Equals(MessageName x, MessageName y)
        {
            return x == y;
        }

        public int GetHashCode(MessageName obj)
        {
            return (int)obj;
        }
    }
}
