
let GenTable = require('./GenTable')
/**
 * 查表法检查胡牌
 */
class HuLogic {
    constructor(opts) {
        this.genTable = GenTable.Instance(); 
    }

    init(){
        return this;
    }


    checkHu(cardInHandList, cardOngoing){

        let cardList = cardInHandList.slice(0);
        if( cardOngoing != undefined){
            cardList.push(cardOngoing);
        }

        let privatePointList = [];
        let ghostList = [];
        for( let i in cardList){
            let cardPoint = cardList[i].privatePoint;
            privatePointList.push(cardPoint);
            if(cardList[i].isGhost){
                ghostList.push(cardPoint);
            }
        }
        return this.isHu(privatePointList,ghostList);
    }

    isHu(cardList, ghostList) {
        // 初始计数
        let cards = [
            [0, 0, 0, 0, 0, 0, 0, 0, 0],// 筒
            [0, 0, 0, 0, 0, 0, 0, 0, 0],// 条
            [0, 0, 0, 0, 0, 0, 0, 0, 0],// 万
            [0, 0, 0, 0, 0, 0, 0, 0, 0] // 字，后两位为0不变，为了保持数组长度一致
        ];
        // 鬼牌总数
        let ghostCount = 0;
        // 计数：统计各牌张数，鬼牌另计在ghostCount上
        for (let card of cardList) {
            if (ghostList.indexOf(card) >= 0) {
                // 是鬼牌
                ghostCount++;
            } else {
                let rank = card % 9;
                let type = Math.floor(card / 9);
                cards[type][rank]++;
            }
        }
        // 开始匹配
        //tmpData是一个3维数组，tmpData[0]是手牌中除去鬼牌后的牌，它可能是筒子,万子，条子，或者字牌)
        //tmpData[1] 是鬼牌的个数
        //tmpData[2] 是否有将
        let tmpData = {};
        tmpData[1] = ghostCount;
        tmpData[2] = false;
        //检查每一个牌型是否胡牌；
        for (let i = 0; i < 4; i++) {
            tmpData[0] = cards[i];
            if (!this.checkCards(tmpData, 0, i==3)) {
                // console.log(array[i], cards[i], false);
                return false;
            }
            // console.log(array[i], cards[i], true);
        }
        if (!tmpData[2] && tmpData[1]%3==2) {
            // 目前匹配没有DD，但剩余鬼牌可匹配到DD
            return true;
        }
        if (tmpData[2] && tmpData[1]%3 == 0) {
            // 匹配到DD了，且剩余鬼牌符合AAA和ABC
            return true;
        }
        return false;
    }

    checkCards(tmpData, ghostCount, feng) {
        let totalCardCount = this.genTable.calTotalCardCount(tmpData[0]);
        if (totalCardCount == 0) {
            return true;
        }
        //查表，如果表里没有，加一个鬼进去再试试
        if (!this.genTable.findCards(tmpData[0], ghostCount, feng)) {
            //递归，每次鬼牌+1,直到鬼牌用尽为止，如果某个牌型把鬼牌用尽都不能胡，那么手牌就不能胡了。
            if (ghostCount < tmpData[1]) {
                return this.checkCards(tmpData, ghostCount+1, feng);
            } else {
                return false;
            }
        } else {//查表找到了，看看牌型是否使用了将。
            //如果该牌型有将
            if ((totalCardCount + ghostCount)%3==2) {
                //之前没用将，直接标记为已使用将了。
                if (!tmpData[2]) {
                    tmpData[2] = true;
                } 
                //如果用将，那么尝试一下再加一个鬼牌进去，看能否胡牌。因为胡牌只能一对将。
                else if (ghostCount < tmpData[1]) {
                    return this.checkCards(tmpData, ghostCount + 1, feng);
                }
                else{//如果找不到就不能胡
                    return false;
                }
            }
            tmpData[1] = tmpData[1] - ghostCount;
            // console.log('判断', tmpData[0], true, ghostCount, tmpData[1]);
        }
        return true;
    }
}

module.exports = HuLogic;