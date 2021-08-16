// 通过数据生成消息卡片的数据
// 消息卡片搭建工具：https://open.feishu.cn/tool/cardbuilder?from=openplantform_mcb_qingfuwu

/**
  * @description 展示一段markdown文本的消息卡片
  * @param {string} label 要展示的文本，支持飞书markdown语法
  * 语法详情参见：https://open.feishu.cn/document/ukTMukTMukTM/uADOwUjLwgDM14CM4ATN
  */
const markdownCard = async (label) => {
  return {
    "config": {
      "wide_screen_mode": true
    },
    "elements": [
      {
        "tag": "div",
        "text": {
          "tag": "lark_md",
          "content": label
        }
      }
    ]
  }
}

/**
  * @description 展示百科搜索结果的消息卡片
  * @param {array} results 搜索的结果
  */
const baikeCard = async (results) => {
  var c = { //将第一条结果（权重最高的）展示出来
    "config": {
      "wide_screen_mode": true
    },
    "elements": [
      {
        "tag": "div",
        "text": {
          "tag": "lark_md",
          "content": `**${results[0].title}**\n${results[0].subtitle}`
        }
      },
      {
        "tag": "div",
        "text": {
          "tag": "lark_md",
          "content": results[0].abstract
        }
      },
      {
        "tag": "div",
        "text": {
          "tag": "lark_md",
          "content": `[百科链接](${results[0].url.replace("m.baike.com","baike.com")})`
        }
      }
    ]
  }
  // 如果关键词是多义项的，会有多个结果。过滤剩余结果中副标题不为空的，并且最多只呈现6个额外义项。
  const others = results.filter(o => o.subtitle.length > 0).slice(1,7)
  if(others.length > 0) {
    c.elements.push({ // 向卡片中插入一条分割线
      "tag": "hr"
    })
    c.elements.push({ // 使用卡片内容模块的fields文本展示额外的义项
      "tag": "div",
      "text": {
        "tag": "lark_md",
        "content": "更多义项："
      },
      "fields": others.map(obj => {
        return {
          "is_short": true,
          "text": {
            "tag": "lark_md",
            "content": `[${obj.subtitle}](${obj.url.replace("m.baike.com","baike.com")})`
          }
        }
      })
    })
  }

  return c
}

module.exports = {
  markdownCard,
  baikeCard
}