/**
 * Created by mac on 2016/11/22.
 */
const Card = require('./Card');
const ModelUtil = require('./ModelUtil');

/**
 * @class Mahjong
 * @mixes
 * 麻将，构造一副麻将牌；筒子，索子，万子，字牌，共34 * 4张.
 * 
 */
class Mahjong {
	constructor() {
		this.assignedIndex = 0; //取牌的位置
		this.retainCardCount = 0; //剩余牌数
		this.cardArray = []; //牌组
		this.hadGetCard = []; //已经取出的牌
		this.ghostCards = []; //红中，白板 点数
		this.middleCard = 0; //翻鬼时，翻开一张牌，这张牌的上一张与下一张为鬼牌，这里保存的是中间翻开的牌.
	}


	/**
	 * init
	 * 初始化一副牌，共34种牌；筒子，索子，万子以及字牌，这里没有花牌
	 */
	init() {
		this.assignedIndex = 0;
		this.cardArray = [];
		this.retainCardCount = 34 * 4;
		for (let i = 0; i < 34; i++) {
			for (let j = 0; j < 4; j++) {
				let card = new Card();
				card.initWithPrivatePoint(i);
				this.cardArray.push(card);
			}
		}

	}

    /**
     * 初始化有鬼牌的麻将。
     * @param {*} ghostCard 标示有哪些鬼牌
     * ghostCard = 0:无鬼
	 * ghostCard = 1:白板鬼
	 * ghostCard = 2: 红中鬼
	 * ghostCard = 4:发财鬼
	 *
	 * ghostCard = 3 = 1&2 白板红中鬼
	 * ghostCard = 5 = 1&4 白板发财鬼
	 * ghostCard = 6 = 2&4 红中发财鬼
	 * ghostCard = 7 = 1&2&4 红中白板发财鬼
	 *
	 * ghostCard = -1:双鬼番鬼
     * 
     */
	initByghostCard(ghostCard) {
		return this.initByGhostType(ghostCard);
	}


    /**
     * 初始化有鬼牌的麻将。
     * 参考initByghostCard
     * @param {*} ghostType 鬼牌类型
     */
	initByGhostType(ghostType) {
		this.assignedIndex = 0;
		this.cardArray = [];
		this.ghostCards = [];
		this.retainCardCount = 34 * 4;

		let bitBai = 1;
		let bitZhong = 1 << 1;
		let bitFa = 1 << 2;

		let ghostCardBaiPrivatePoint = CARDID.CARDID_BAI; //白板
		let ghostCardZhongPrivatePoint = CARDID.CARDID_ZHONG; //红中
		let ghostCardFaPrivatePoint = CARDID.CARDID_FA;

		//翻鬼
		if (ghostType == -1) {
			let tempCards = Mahjong.RandomGhostPoint(3);

			this.ghostCards.push(tempCards[0]); //鬼牌
			this.ghostCards.push(tempCards[2]); //鬼牌
			this.middleCard = tempCards[1]; //中间牌
		} else {
			if ((bitBai & ghostType) == bitBai) {
				this.ghostCards.push(ghostCardBaiPrivatePoint)
			}

			if ((bitZhong & ghostType) == bitZhong) {
				this.ghostCards.push(ghostCardZhongPrivatePoint)
			}

			if ((bitFa & ghostType) == bitFa) {
				this.ghostCards.push(ghostCardFaPrivatePoint);
			}
		}

		for (let i = 0; i < 34; i++) {
			for (let j = 0; j < 4; j++) {
				let card = new Card();
				card.initWithPrivatePoint(i, this.ghostCards);
				this.cardArray.push(card);
			}
		}
	}


	/**
	 * initWithGhostBaiORZhong
	 * 初始化一副牌，指定白板是否鬼牌，红中是否鬼牌
	 * @param {bool} isGhostBai true:白板为鬼牌 false:白板不是鬼牌
	 * @param {bool} isGhostZhong true:红中为鬼牌 false:红中不是鬼牌
	 */
	initWithGhostBaiORZhong(isGhostBai, isGhostZhong, isGhostFa) {
		this.assignedIndex = 0;
		this.cardArray = [];
		this.ghostCards = [];
		this.retainCardCount = 34 * 4;

		let ghostCardBaiPrivatePoint = CARDID.CARDID_BAI; //白板
		let ghostCardZhongPrivatePoint = CARDID.CARDID_ZHONG; //红中
		let ghostCardFaPrivatePoint = CARDID.CARDID_FA;

		if (isGhostBai) {
			this.ghostCards.push(ghostCardBaiPrivatePoint)
		}

		if (isGhostZhong) {
			this.ghostCards.push(ghostCardZhongPrivatePoint)
		}

		if (isGhostFa) {
			this.ghostCards.push(ghostCardFaPrivatePoint);
		}

		for (let i = 0; i < 34; i++) {
			for (let j = 0; j < 4; j++) {
				let card = new Card();
				card.initWithPrivatePoint(i, this.ghostCards);
				this.cardArray.push(card);
			}
		}

	};

	//洗牌
	/**
	 * shuffle
	 * 洗牌
	 * 把遍历cardArray中的每一张牌与任意位置中的一张牌交换
	 */
	shuffle() {

		for (let i = 0; i < this.cardArray.length; i++) {
			let card = this.cardArray[i];

			let randomPosition = getRandom() % this.cardArray.length;
			let pcard = this.cardArray[randomPosition];
			this.cardArray[i] = pcard;
			this.cardArray[randomPosition] = card;
		}
		// let pointList = this.cardArray.map((card) => {
		// 	return card.privatePoint;
		// })
	}

	shuffleSpecial(pointList) {

		//1条, 9筒,4万,2条,1万,9筒,1条,东,9万,6条,7筒,4条,8筒,2条,
		// 西,2万,1筒,8筒,8万,白,白,6条,3条,1筒,7万,南,北, 8条,2筒,
		//1筒,3条,5万,5条,4条,白,9万,4条,南,5万,2万, 9筒,1条,4筒,发,5条
		//,7万,发,8条,南,7万,7万,8条,1万, 东,6万,8筒,3筒,6筒,2条,4筒,
		//8万,5条,8条,4万,8万,4万, 1万,9条,4筒,3万,6万,8万,东,9万,西,2筒,
		//中,2万,2万, 4万,中,白,7条,7条,东,5万,北,6筒,4条,6条,7筒,9万,
		// 2筒,西,7筒,5筒,9条,6万,3筒,9条,7条,6万,中,3筒,北, 西,1万,6筒,
		//9条,5筒,3筒,5筒,3万,8筒,5筒,9筒,2筒,3万, 3万,3条,3条,7筒,6条,
		//中,2条,4筒,5条,7条,南,发,5万, 北,1条,1筒,发,6筒,
		let tempList = [9, 8, 21, 10, 18, 8, 9, 27, 26, 14, 6, 12, 7, 10,
			29, 19, 0, 7, 25, 33, 33, 14, 11, 0, 24, 28, 30, 16, 1, 0,
			11, 22, 13, 12, 33, 26, 12, 28, 22, 19, 8, 9, 3, 32, 13,
			24, 32, 16, 28, 24, 24, 16, 18, 27, 23, 7, 2, 5, 10, 3,
			25, 13, 16, 21, 25, 21, 18, 17, 3, 20, 23, 25, 27, 26,
			29, 1, 31, 19, 19, 21, 31, 33, 15, 15, 27, 22, 30, 5,
			12, 14, 6, 26, 1, 29, 6, 4, 17, 23, 2, 17, 15, 23, 31, 2,
			30, 29, 18, 5, 17, 4, 2, 4, 20, 7, 4, 8, 1, 20, 20, 11, 11,
			6, 14, 31, 10, 3, 13, 15, 28, 32, 22, 30, 9, 0, 32, 5];
		if (pointList != undefined) {
			tempList = pointList;
		}

		this.cardArray = [];
		for (let i = 0; i < tempList.length; i++) {
			let card = Card.CreateCardWithPoint(tempList[i], this.ghostCards.indexOf(i) >= 0);
			this.cardArray.push(card);
		}

		console.log("*****正在使用测试用的特殊牌型*****")
	}


    /**
     * 获取指定的牌数据，这个函数一般用于测试
     * @param {*} displayNameList 牌名数组["1筒","2筒"..]
     * @param {*} fillCount 如果找不到够的这些牌，也必须填补到这个张数，让返回的牌数个数与fillcount一致
     */
	getMysetCards(displayNameList, fillCount = 13) {

		let list = []
		for (let i in displayNameList) {
			for (let j = this.assignedIndex; j < this.cardArray.length; j++) {
				if (displayNameList[i] == this.cardArray[j].displayName) {
					list.push(this.cardArray[j])
					this.cardArray.splice(j, 1);
					break
				}
			}
		}
		this.retainCardCount -= list.length;

		if (list.length < fillCount) {

			let loopCount = fillCount - list.length;
			for (; loopCount > 0; loopCount--) {
				let card = this.assigningOneCard();
				list.push(card);
			}
		}
		list = ModelUtil.sort(list);
		return list
	}

	/**
	 * assigningCards 取多张牌，一般为1手牌，13张
	 * 发出多张牌
	 * @param {Int} requestCardCount 请求发牌的张数
	 * 当一副牌中不够发的时候,取剩余牌
	 * @return {Array} 返回发出的牌队列，对象为Card
	 */
	assigningRemainCard(requestCardCount) {
		if (requestCardCount < 1) {
			return [];
		}
		if ((this.assignedIndex + requestCardCount) > this.cardArray.length) {
			requestCardCount = this.cardArray.length - this.assignedIndex;
		}

		let result = this.cardArray.slice(this.assignedIndex, this.assignedIndex + requestCardCount);

		//记录发出的牌
		this.hadGetCard = this.hadGetCard.concat(result);

		this.assignedIndex += requestCardCount;
		this.retainCardCount -= requestCardCount;
		if (this.retainCardCount <= 0) {
			this.retainCardCount = 0;
		}
		result = ModelUtil.sort(result);
		return result;
	}


	//取多张牌，一般为1手牌，13张
	/**
	 * assigningCards
	 * 发出多张牌
	 * @param {Int} requestCardCount 请求发牌的张数
	 * 当一副牌中不够发的时候，返回值为空队列
	 * @return {Array} 返回发出的牌队列，对象为Card
	 */
	assigningCards(requestCardCount) {
		if (requestCardCount < 1) {
			return [];
		}
		if ((this.assignedIndex + requestCardCount) > this.cardArray.length) {
			return [];
		}

		let result = this.cardArray.slice(this.assignedIndex, this.assignedIndex + requestCardCount);

		//记录发出的牌
		this.hadGetCard = this.hadGetCard.concat(result);

		this.assignedIndex += requestCardCount;
		this.retainCardCount -= requestCardCount;
		result = ModelUtil.sort(result);
		return result;
	}

	//获取1张牌
	/**
	 * assigningOneCard
	 * 发出一张牌
	 * @return {Object}  返回一个Card对象
	 */
	assigningOneCard() {
		let cardList = this.assigningCards(1);
		if (cardList.length === 0) {
			return null;
		}

		let card = cardList[0];
		return card;
	}


	/**
	 * getRemindCardCount
	 * 获取剩余多少张牌还没有发出
	 * @return {Int} 剩余多少张牌没有发出
	 */
	getRemindCardCount() {
		return this.cardArray.length - this.assignedIndex;
	}

	/**
	 * getAssignedCardList
	 * 获取已发出牌的列表
	 * @return {Array} 已发出牌的列表, 列表中为Card对象
	 */
	getAssignedCardList() {
		this.hadGetCard = ModelUtil.sort(this.hadGetCard);
		return this.hadGetCard;
	}

    /**
    * 随机返回3张连续的牌,并且保证3张牌花色是一样的。
    */
	static RandomGhostPoint(count) {
		//如果产生鬼的个数少于1个，或者大于7个，都视为不合理
		if (count <= 0 || count > 7) {
			return [];
		}

		let result = [];
		//0〜8:表示1~9筒；9〜17:表示1〜9索；18〜26:1〜9万；27,28,29,30:东南西北 31~33:中发白
		let nRandom = Math.floor(Math.random() * 10000);
		let otherRandom2 = Math.floor(Math.random() * 100);
		let otherRandom3 = Math.floor(Math.random() * 100);

		let firstCard = (nRandom * otherRandom2 * otherRandom3) % 34;

		let card = new Card();
		card.initWithPrivatePoint(firstCard);
		result.push(card.privatePoint);

		for (let i = 1; i < count; i++) {
			let cardNext = card.getNextCard();
			result.push(cardNext.privatePoint);
			card = cardNext;
		}

		return result;
	}
}

module.exports = Mahjong;



/**
 * @return {Int} 返回随机数
 */
function getRandom() {
	return Math.floor(Math.random() * 10000);
}