// util 代码模块都是工具方法，每个方法都是最小单元功能，可根据情况按需引入
//
// 更多开放平台API请参见 https://open.feishu.cn/document/ukTMukTMukTM/uITNz4iM1MjLyUzM

/**
 * @description: 获取 tenant_access_token
 * @return {string} tenant_access_token
 * 详见文档：https://open.feishu.cn/document/ukTMukTMukTM/uIjNz4iM2MjLyYzM
 */
const getTenantToken = async () => {
  const res = await axios.post('https://open.feishu.cn/open-apis/auth/v3/tenant_access_token/internal/', {
    "app_id": lark.env.APP_ID,
    "app_secret": lark.env.APP_SECRET,
  });

  return res.data.tenant_access_token;
};

/**
 * @description: 发送文本消息
 * @param {string} chatId 群 ID，需要在群内发送消息时传入
 * @param {string} rootId 要回复的消息 ID，需要对一条消息进行回复时传入
 * @param {string} text 消息文本内容
 * @param {string} openId 用户 ID，需要对用户私聊时传入
 * 详见文档：https://open.feishu.cn/document/ukTMukTMukTM/uUjNz4SN2MjL1YzM
 */
const sendTextMessage = async ({ chatId, text, rootId, openId }) => {
  const tenantToken = await getTenantToken()

  const data = {
    chat_id: chatId,
    msg_type: 'text',
    content: {
      text,
    },
  }

  // 回复消息
  if (rootId) {
    data.root_id = rootId;
  }
  // 在群内发消息
  if (chatId) {
    data.chat_id = chatId;
  }
  // 私聊用户
  if (openId) {
    data.open_id = openId;
  }
  const res = await axios({
    method: 'post',
    url: 'https://open.feishu.cn/open-apis/message/v4/send',
    headers: {
      'Authorization': `Bearer ${tenantToken}`,
    },
    data,
  })
}

/**
 * @description: 发送消息卡片
 * @param {string} chatId 群 ID，需要在群内发送消息时传入
 * @param {string} rootId 要回复的消息 ID，需要对一条消息进行回复时传入
 * @param {string} card   消息卡片内容，是一段Json，可通过搭建工具生成 https://open.feishu.cn/tool/cardbuilder?from=openplantform_mcb_qingfuwu
 * @param {string} openId 用户 ID，需要对用户私聊时传入
 * 详见文档：https://open.feishu.cn/document/ukTMukTMukTM/uYTNwUjL2UDM14iN1ATN
 */
const sendCardMessage = async ({ chatId, card, rootId, openId }) => {
  const tenantToken = await getTenantToken()

  const data = {
    chat_id: chatId,
    msg_type: 'interactive',
    card
  }

  // 回复消息
  if (rootId) {
    data.root_id = rootId;
  }
  // 在群内发消息
  if (chatId) {
    data.chat_id = chatId;
  }
  // 私聊用户
  if (openId) {
    data.open_id = openId;
  }
  const res = await axios({
    method: 'post',
    url: 'https://open.feishu.cn/open-apis/message/v4/send',
    headers: {
      'Authorization': `Bearer ${tenantToken}`,
    },
    data,
  })
}

/**
 * @description: 发送多条文本消息，支持频控的批量发送
 * @param {array} objArray 群 ID 和文本数组
 */
const sendTextMessageToMultipleChats = async (objArray) => {
  // 每次获取一下最新的 tenant_access_token
  const tenantToken = await getTenantToken();
  const maxLength = 50;

  const send = async (i, t) => {
    // 发送的文本、群 ID
    const { text, chatId } = objArray[i];

    setTimeout(async() => {
      sendTextMessage({ text, chatId, tenantToken });
    }, t);
  };
  for (let i = 0; i < objArray.length; i++) {
    // 每 maxLength 个每秒一起发送
    send(i, Math.floor(i / maxLength) * 500);
  }
}

/**
 * @description: 获取所有机器人所在群
 * @return {array} 群列表
 * https://open.feishu.cn/document/ukTMukTMukTM/uITO5QjLykTO04iM5kDN
 */
const getAllChats = async () => {
  let hasMore = true;
  let groups = [];
  let pageToken = '0';
  let url = 'https://open.feishu.cn/open-apis/chat/v4/list';

  // 每次获取一下最新的 tenant_access_token
  const tenantToken = await getTenantToken();

  while (hasMore) {
    const res = await axios({
      method: 'get',
      url,
      headers: {
        'Authorization': `Bearer ${tenantToken}`,
      },
    });

    hasMore = res.data.data.has_more;
    url = `${url}?page_token=${res.data.data.page_token}`;
    groups = groups.concat(res.data.data.groups);
  }

  return groups;
}

/**
 * @description: 用 email 换取 userId 和 openId
 * @return {object} openId 和 userId
 * https://open.feishu.cn/document/ukTMukTMukTM/uEDMwUjLxADM14SMwATN
 */
const getUserIdByEmail = async (email) => {
  const tenantToken = await getTenantToken();
  const res = await axios({
    method: 'post',
    url: 'https://open.feishu.cn/open-apis/user/v4/email2id',
    headers: {
      'Authorization': `Bearer ${tenantToken}`,
    },
    data: {
      email,
    },
  });

  return res.data.data
}

/**
 * @description: 获取群信息
 * @return {object} 群信息（群的基础信息、群成员
 * https://open.feishu.cn/document/ukTMukTMukTM/uMTO5QjLzkTO04yM5kDN
 */
const getGroupInfo = async (chatId) => {
  const tenantToken = await getTenantToken();
  const res = await axios({
    method: 'get',
    url: `https://open.feishu.cn/open-apis/chat/v4/info?chat_id=${chatId}`,
    headers: {
      'Authorization': `Bearer ${tenantToken}`,
    },
  });

  return res.data.data
}

/**
 * @description: 获取用户信息
 * @param {string} openId 用户的 openId
 * @return {object} 用户信息
 * https://open.feishu.cn/document/ukTMukTMukTM/ukjMwUjL5IDM14SOyATN
 */
const getUserInfo = async (openId) => {
  const tenantToken = await getTenantToken();
  const res = await axios({
    method: 'get',
    url: `https://open.feishu.cn/open-apis/user/v4/info?open_id=${openId}`,
    headers: {
      'Authorization': `Bearer ${tenantToken}`,
    },
  })

  return res.data.data;
}

module.exports = {
  getTenantToken,
  sendTextMessage,
  sendTextMessageToMultipleChats,
  getAllChats,
  getUserIdByEmail,
  getGroupInfo,
  getUserInfo,
  sendCardMessage,
}