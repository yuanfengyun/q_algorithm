
const fs = require('fs');

class GenTable {

    constructor(opts) {


        // 普通牌：万，筒，条
        this.KeyDic = {};
        this.KeyDicWithGhost = {};
        for (let i = 1; i < 9; i++) {
            this.KeyDicWithGhost[i] = {};
        }
        // 字牌
        this.KeyDicFeng = {};
        this.KeyDicFengWithGhost = {};
        for (let i = 1; i < 9; i++) {
            this.KeyDicFengWithGhost[i] = {};
        }
    }

    static Instance() {
        if (!GenTable.instance) {
            let instance = new GenTable();
            instance.init();
            GenTable.instance = instance;
        }
        return GenTable.instance;
    }

    init() {
        if (!fs.existsSync(__dirname + '/tbl/' + 'Table0')) {
            this.start();
        } else {
            this.load();
        }
    }

    findCards(cards, ghostCount, isFeng) {
        let key = this.generateKey(cards);
        if (ghostCount > 0) {
            if (isFeng && this.KeyDicFengWithGhost[ghostCount][key]) {
                return true;
            }
            if (!isFeng && this.KeyDicWithGhost[ghostCount][key]) {
                return true;
            }
        } else if (isFeng && this.KeyDicFeng[key]) {
            return true;
        } else if (!isFeng && this.KeyDic[key]) {
            return true;
        }
        return false;
    }

    start() {
        let cards = [0, 0, 0, 0, 0, 0, 0, 0, 0];
        let level = 0;
        let eye = false;
        // 非字牌
        let feng = false;
        // 生成不带鬼穷举表
        this.genTable(cards, level, eye, feng);
        // 生成带鬼穷举表
        this.genTableWithGhost(feng);
        // 字牌
        feng = true;
        // 生成不带鬼穷举表
        this.genTable(cards, level, eye, feng);
        // 生成带鬼穷举表
        this.genTableWithGhost(feng);
        // 保存
        this.dump();
    }

    dump() {
        // 不带鬼
        this._dump('Table0', this.KeyDic);
        // 带鬼
        for (let i in this.KeyDicWithGhost) {
            this._dump('Table' + i, this.KeyDicWithGhost[i]);
        }
        // 字牌不带鬼
        this._dump('Table_Feng0', this.KeyDicFeng);
        // 字牌带鬼
        for (let i in this.KeyDicFengWithGhost) {
            this._dump('Table_Feng' + i, this.KeyDicFengWithGhost[i]);
        }
    }

    _dump(fileName, keyDic) {
        var fWrite = fs.createWriteStream(__dirname + '/tbl/' + fileName);
        for (let key in keyDic) {
            fWrite.write(key, 'utf8');
            fWrite.write('\n', 'utf8');
        }
        fWrite.end();

        fWrite.on('finish', function () {
            console.log('写入完成');
        })
    }

    load() {
        // 不带鬼
        this._load('Table0', this.KeyDic);
        // 带鬼
        for (let i = 1; i < 9; i++) {
            this._load('Table' + i, this.KeyDicWithGhost[i]);
        }
        // 字牌不带鬼
        this._load('Table_Feng0', this.KeyDicFeng);
        // 字牌带鬼
        for (let i = 1; i < 9; i++) {
            this._load('Table_Feng' + i, this.KeyDicFengWithGhost[i]);
        }
    }

    _load(fileName, keyDic) {
        if (!fs.existsSync(__dirname + '/tbl/' + fileName)) {
            console.log(fileName, "文件不存在");
            return;
        }

        const fileDoc = fs.readFileSync(__dirname + '/tbl/' + fileName);
        const keyArray = String(fileDoc).split('\n');
        for (let i = 0; i < keyArray.length; i++) {
            let key = keyArray[i];
            if (key) {
                keyDic[key] = 1;
            }
        }
    }

    genTableWithGhost(feng) {
        let dic = this.KeyDic;
        if (feng) {
            dic = this.KeyDicFeng;
        }
        for (let key in dic) {
            let cards = this.toNumberArray(key);
            this.generateWithGhost(cards, 1, feng);
        }
    }

    generateWithGhost(cards, ghostCount, feng) {
        for (let i = 0; i < 9; i++) {
            if (cards[i] == 0) continue;
            cards[i]--;
            if (!this.tryAdd(cards, ghostCount, feng)) {
                cards[i]++;
                continue;
            }
            if (ghostCount < 8) {
                this.generateWithGhost(cards, ghostCount + 1, feng);
            }
            cards[i]++;
        }
    }
    // 判断是否符合规则
    tryAdd(cards, ghostCount, feng) {
        for (let i = 0; i < 9; i++) {
            if (cards[i] < 0 || cards[i] > 4) {
                return false;
            }
        }
        let key = this.generateKey(cards);
        if (feng && this.KeyDicFengWithGhost[ghostCount][key]) {
            return false;
        }
        if (!feng && this.KeyDicWithGhost[ghostCount][key]) {
            return false;
        }
        if (feng) {
            this.KeyDicFengWithGhost[ghostCount][key] = 1;
        } else {
            this.KeyDicWithGhost[ghostCount][key] = 1;
        }
        return true;
    }
    // 生成不带鬼的穷举表
    genTable(cards, level, eye, feng) {
        for (let i = 0; i < 9; i++) {
            if (feng && i > 6) continue;
            let totalCardCount = this.calTotalCardCount(cards);
            // +AAA，递归
            if (totalCardCount <= 11 && cards[i] <= 1) {
                cards[i] += 3;
                let key = this.generateKey(cards);
                if (feng && this.KeyDicFeng[key] == null) {
                    this.KeyDicFeng[key] = 4;
                }
                if (!feng && this.KeyDic[key] == null) {
                    this.KeyDic[key] = 4;
                }
                if (level < 5) {
                    this.genTable(cards, level + 1, eye, feng);
                }
                cards[i] -= 3;
            }
            // +ABC，递归
            if (!feng && totalCardCount <= 11 && i < 7 && cards[i] <= 3 && cards[i + 1] <= 3 && cards[i + 2] <= 3) {
                cards[i] += 1;
                cards[i + 1] += 1;
                cards[i + 2] += 1;
                let key = this.generateKey(cards);
                if (this.KeyDic[key] == null) {
                    this.KeyDic[key] = 4;
                }
                if (level < 5) {
                    this.genTable(cards, level + 1, eye, feng);
                }
                cards[i] -= 1;
                cards[i + 1] -= 1;
                cards[i + 2] -= 1;
            }
            // +DD，递归
            if (totalCardCount <= 12 && cards[i] <= 2 && !eye) {
                cards[i] += 2;
                let key = this.generateKey(cards);
                if (feng && this.KeyDicFeng[key] == null) {
                    this.KeyDicFeng[key] = 4;
                }
                if (!feng && this.KeyDic[key] == null) {
                    this.KeyDic[key] = 4;
                }
                if (level < 5) {
                    this.genTable(cards, level + 1, true, feng);
                }
                cards[i] -= 2;
            }
        }
    }

    toNumberArray(strKey) {
        let result = [];
        for (let i = 0; i < strKey.length; i++) {
            let temp = strKey.slice(i, i + 1);
            result.push(parseInt(temp));
        }
        return result;
    }

    generateKey(cards) {
        let key = '';
        let dic = ['0', '1', '2', '3', '4']
        for (let i = 0; i < cards.length; i++) {
            key = key + dic[cards[i]];
        }
        return key;
    }

    calTotalCardCount(cards) {
        let count = 0;
        for (let i = 0; i < 9; i++) {
            count += cards[i];
        }
        return count;
    }
}

module.exports = GenTable;