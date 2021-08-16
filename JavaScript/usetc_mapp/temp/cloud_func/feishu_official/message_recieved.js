/**
  * @description 处理接收消息事件
  * @param {object} params 事件推送的数据
  * @param {object} context 调用上下文
  * 详见文档：https://open.feishu.cn/document/ukTMukTMukTM/uMTNxYjLzUTM24yM1EjN#接收消息
  */

const utils = require('./utils');
const card_template = require('./card_template');

module.exports = async function(params, context) {

  const {event} = params

  if(event.type != 'message') {
    throw '收到的不是消息事件，请检查事件订阅与云函数的绑定关系'
  }

  switch (event.msg_type) { // 按照消息的类型进行处理
    case 'text': // 文本消息
      await handle_text_received(event)
      break
    case 'image': // 图片消息
      await handle_image_received(event)
      break
    default:
      throw `未支持的消息类型： ${event.type}.`
      // 更多消息类型可以参见https://open.feishu.cn/document/ukTMukTMukTM/uMTNxYjLzUTM24yM1EjN#接收消息
  }
}

/**
  * @description 处理接收到的文本消息
  * @param {object} event 消息事件数据
  * 详见文档：https://open.feishu.cn/document/ukTMukTMukTM/uMTNxYjLzUTM24yM1EjN#文本消息
  */
const handle_text_received = async (event) => {
  const { open_id,          // 发送此消息的用户的id
        open_chat_id,       // 消息所在的会话的id
        open_message_id,    // 消息的id
        chat_type,          // private - 私聊机器人，group - 群聊中@机器人
        text_without_at_bot // 消息内容，过滤了@机器人的文本
  } = event

  try {
    // 将用户发送的消息作为关键词提取出来
    const keyword = text_without_at_bot.replace(" ","")
    var card

    // 通过【头条百科】(www.baike.com) 查询发送给机器人的关键词
    // 头条百科的接口通过飞书开放平台开放，需要tenant access token鉴权
    const tenantToken = await utils.getTenantToken()
    const res = await axios({
      method: 'get',
      url: `https://open.feishu.cn/open-apis/baike/wiki`,
      params: { query: keyword },
      headers: {
        'Authorization': `Bearer ${tenantToken}`,
      },
    })

    // 获取查询结果并通过工具方法生成消息卡片
    const results = res.data.data.wiki_doc_list
    if (results) {
      card = await card_template.baikeCard(results)
    }else {
      card = await card_template.markdownCard(`抱歉，没有找到与 “**${keyword}**” 相关的结果。`)
    }

    // 回复收到的消息
    if (chat_type == "private") {
      // 收到私聊消息
      await utils.sendCardMessage({
        card: card,
        openId: open_id
      });
    }else {
      // 收到群聊消息。回复时，还会@发消息给机器人的用户
      await utils.sendCardMessage({
        chatId: open_chat_id,
        card:   card,
        rootId: open_message_id, // 指定要回复的消息（这里回复用户在群里@机器人的那条消息）
        openId: open_id
      });
    }
  }catch(err) {
    console.log(`回复文本消息失败：`, err, event)
  }
}

/**
  * @description 处理接收到的图片消息：当收到一张图片时，私聊回复图片的key给发送者
  * @param {object} event 消息事件数据
  * 详见文档：https://open.feishu.cn/document/ukTMukTMukTM/uMTNxYjLzUTM24yM1EjN#图片image消息
  */
const handle_image_received = async (event) => {
  const { open_id,  // 发送此消息的用户的id
          image_key // 图片的key
  } = event

  await utils.sendTextMessage({
    text: `这张图片的Key是：\n\n ${image_key}`,
    openId: open_id
  });
}

