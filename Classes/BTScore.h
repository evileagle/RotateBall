//
//  BTScore.h
//  Ballthrow
//
//  Created by 李春雨 on 14/12/6.
//
//

#ifndef __Ballthrow__BTScore__
#define __Ballthrow__BTScore__

class BTScore {
public:
    static BTScore* getInstance();
    int getScore();
    int addScore(int score);
    void resetScore();
    int getBest();
    bool isNewBest();
private:
    BTScore();
    static BTScore* s_instance;
    int _score;
    int _best;
    bool _is_new_best;
};

#endif /* defined(__Ballthrow__BTScore__) */
