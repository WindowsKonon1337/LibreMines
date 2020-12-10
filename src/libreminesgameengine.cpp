/*****************************************************************************
 * LibreMines                                                                *
 * Copyright (C) 2020  Bruno Bollos Correa                                   *
 *                                                                           *
 * This program is free software: you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *****************************************************************************
 */


#include "libreminesgameengine.h"

#include <QRandomGenerator>
#include <QTimer>

LibreMinesGameEngine::CellGameEngine::CellGameEngine():
    state(ZERO),
    isHidden(true),
    hasFlag(false)
{

}

LibreMinesGameEngine::LibreMinesGameEngine()
{

}

void LibreMinesGameEngine::vNewGame(const uchar _X,
                                    const uchar _Y,
                                    ushort i_nMines_,
                                    const uchar i_X_Clean,
                                    const uchar i_Y_Clean)
{
    iX = _X;
    iY = _Y;
    nMines = i_nMines_;
    iMinesLeft = i_nMines_;
    iHiddenCells = 0;

    const bool bRemakingGame = (i_X_Clean != 255 && i_Y_Clean != 255);

    if(!bRemakingGame)
    {
        bGameActive = false;
        bFirst = true;
        principalMatrix = std::vector<std::vector<CellGameEngine>> (iX, std::vector<CellGameEngine>(iY));
    }


    if(bRemakingGame)
    {
        for(std::vector<CellGameEngine>& i: principalMatrix)
        {
            for(CellGameEngine& j: i)
            {
                j.state = ZERO;
            }
        }
    }
    else
    {
        for(uchar j=0; j<iY; j++)
        {
            for (uchar i=0; i<iX; i++)
            {
                CellGameEngine& cell = principalMatrix[i][j];

                cell.state = ZERO;
                cell.isHidden = true;
                cell.hasFlag = false;

//                qApp->processEvents();
            }
        }
    }


    QVector<Vector2Dshort> vt_vt2d_CleanPoints = QVector<Vector2Dshort>();

    if(bRemakingGame)
    {
        vt_vt2d_CleanPoints.reserve(9);

        for (short i=-1; i<=1; i++)
        {
            for (short j=-1; j<=1; j++)
            {
                vt_vt2d_CleanPoints.append(Vector2Dshort(i_X_Clean + i, i_Y_Clean + j));
            }
        }
    }

    while(i_nMines_ > 0)
    {
        uchar i = QRandomGenerator::global()->bounded(0, iX);
        uchar j = QRandomGenerator::global()->bounded(0, iY);

        if(bRemakingGame)
        {
            bool forbidden = false;
            for (const Vector2Dshort& n: vt_vt2d_CleanPoints)
            {
                if(n.x == i &&
                   n.y == j)
                {
                    forbidden = true;
                    break;
                }
            }
            if(forbidden)
                continue;
        }

        CellGameEngine& cell = principalMatrix[i][j];

        if(cell.state == ZERO)
        {
            i_nMines_--;
            cell.state = MINE;
        }
    }

    for(uchar j=0; j<iY; j++)
    {
        for (uchar i=0; i<iX; i++)
        {
            CellGameEngine& cell = principalMatrix[i][j];

            if(cell.state == ZERO)
            {
                iHiddenCells++;

                uchar minesNeighbors = 0;

                if(i == 0 &&
                   j == 0)
                {
                    if(principalMatrix[i+1][j].state == MINE)
                        minesNeighbors++;
                    if(principalMatrix[i][j+1].state == MINE)
                        minesNeighbors++;
                    if(principalMatrix[i+1][j+1].state == MINE)
                        minesNeighbors++;
                }
                else if(i == 0 &&
                        j == iY-1)
                {
                    if(principalMatrix[i+1][j].state == MINE)
                        minesNeighbors++;
                    if(principalMatrix[i][j-1].state == MINE)
                        minesNeighbors++;
                    if(principalMatrix[i+1][j-1].state == MINE)
                        minesNeighbors++;
                }
                else if(i == iX-1 &&
                        j==0)
                {
                    if(principalMatrix[i-1][j].state == MINE)
                        minesNeighbors++;
                    if(principalMatrix[i][j+1].state == MINE)
                        minesNeighbors++;
                    if(principalMatrix[i-1][j+1].state == MINE)
                        minesNeighbors++;
                }
                else if(i == iX-1 &&
                        j == iY-1)
                {
                    if(principalMatrix[i-1][j].state == MINE)
                        minesNeighbors++;
                    if(principalMatrix[i][j-1].state == MINE)
                        minesNeighbors++;
                    if(principalMatrix[i-1][j-1].state == MINE)
                        minesNeighbors++;
                }
                else if(i == 0 &&
                        j > 0 &&
                        j < iY-1)
                {
                    if(principalMatrix[i+1][j].state == MINE)
                        minesNeighbors++;
                    if(principalMatrix[i][j+1].state == MINE)
                        minesNeighbors++;
                    if(principalMatrix[i+1][j+1].state == MINE)
                        minesNeighbors++;
                    if(principalMatrix[i][j-1].state == MINE)
                        minesNeighbors++;
                    if(principalMatrix[i+1][j-1].state == MINE)
                        minesNeighbors++;
                }
                else if(i == iX-1 &&
                        j >0 &&
                        j < iY-1)
                {
                    if(principalMatrix[i-1][j].state == MINE)
                        minesNeighbors++;
                    if(principalMatrix[i][j+1].state == MINE)
                        minesNeighbors++;
                    if(principalMatrix[i-1][j+1].state == MINE)
                        minesNeighbors++;
                    if(principalMatrix[i][j-1].state == MINE)
                        minesNeighbors++;
                    if(principalMatrix[i-1][j-1].state == MINE)
                        minesNeighbors++;
                }
                else if(i > 0 &&
                        i < iX-1 &&
                        j == 0){
                    if(principalMatrix[i-1][j].state == MINE)
                        minesNeighbors++;
                    if(principalMatrix[i+1][j].state == MINE)
                        minesNeighbors++;
                    if(principalMatrix[i-1][j+1].state == MINE)
                        minesNeighbors++;
                    if(principalMatrix[i][j+1].state == MINE)
                        minesNeighbors++;
                    if(principalMatrix[i+1][j+1].state == MINE)
                        minesNeighbors++;
                }
                else if(i > 0 &&
                        i < iX-1 &&
                        j == iY-1)
                {
                    if(principalMatrix[i+1][j].state == MINE)
                        minesNeighbors++;
                    if(principalMatrix[i-1][j].state == MINE)
                        minesNeighbors++;
                    if(principalMatrix[i-1][j-1].state == MINE)
                        minesNeighbors++;
                    if(principalMatrix[i][j-1].state == MINE)
                        minesNeighbors++;
                    if(principalMatrix[i+1][j-1].state == MINE)
                        minesNeighbors++;
                }
                else
                {
                    if(principalMatrix[i-1][j-1].state == MINE)
                        minesNeighbors++;
                    if(principalMatrix[i-1][j].state == MINE)
                        minesNeighbors++;
                    if(principalMatrix[i-1][j+1].state == MINE)
                        minesNeighbors++;
                    if(principalMatrix[i][j-1].state == MINE)
                        minesNeighbors++;
                    if(principalMatrix[i][j+1].state == MINE)
                        minesNeighbors++;
                    if(principalMatrix[i+1][j-1].state == MINE)
                        minesNeighbors++;
                    if(principalMatrix[i+1][j].state == MINE)
                        minesNeighbors++;
                    if(principalMatrix[i+1][j+1].state == MINE)
                        minesNeighbors++;
                }

                cell.state = (CELL_STATE)minesNeighbors;
            }
        }
    }
    iCellsToUnlock = iHiddenCells;
    bGameActive = true;

    if(bRemakingGame)
    {
        emit SIGNAL_remakeGame();
    }
}

void LibreMinesGameEngine::vResetPrincipalMatrix()
{
    principalMatrix.clear();
}

void LibreMinesGameEngine::vCleanCell(const uchar _X, const uchar _Y)
{
    if(!principalMatrix[_X][_Y].hasFlag &&
       principalMatrix[_X][_Y].state == MINE)
    {
        vGameLost(_X, _Y);
    }
    else if(principalMatrix[_X][_Y].isHidden &&
            !principalMatrix[_X][_Y].hasFlag)
    {
        principalMatrix[_X][_Y].isHidden = false;
        emit SIGNAL_showCell(_X, _Y);

        if(principalMatrix[_X][_Y].state == ZERO)
        {
            if(_X == 0 &&
               _Y == 0)
            {
                if(principalMatrix[_X+1][_Y].isHidden)
                    vCleanCell(_X+1, _Y);
                if(principalMatrix[_X][_Y+1].isHidden)
                    vCleanCell(_X, _Y+1);
                if(principalMatrix[_X+1][_Y+1].isHidden)
                    vCleanCell(_X+1, _Y+1);
            }
            else if(_X == 0 &&
                    _Y == iY-1)
            {
                if(principalMatrix[_X+1][_Y].isHidden)
                    vCleanCell(_X+1, _Y);
                if(principalMatrix[_X][_Y-1].isHidden)
                    vCleanCell(_X, _Y-1);
                if(principalMatrix[_X+1][_Y-1].isHidden)
                    vCleanCell(_X+1, _Y-1);
            }
            else if(_X == iX-1 &&
                    _Y == 0)
            {
                if(principalMatrix[_X-1][_Y].isHidden)
                    vCleanCell(_X-1, _Y);
                if(principalMatrix[_X][_Y+1].isHidden)
                    vCleanCell(_X, _Y+1);
                if(principalMatrix[_X-1][_Y+1].isHidden)
                    vCleanCell(_X-1, _Y+1);
            }
            else if(_X == iX-1 &&
                    _Y == iY-1)
            {
                if(principalMatrix[_X-1][_Y].isHidden)
                    vCleanCell(_X-1, _Y);
                if(principalMatrix[_X][_Y-1].isHidden)
                    vCleanCell(_X, _Y-1);
                if(principalMatrix[_X-1][_Y-1].isHidden)
                    vCleanCell(_X-1, _Y-1);
            }
            else if(_X == iX-1 &&
                    _Y == 0)
            {
                if(principalMatrix[_X-1][_Y].isHidden)
                    vCleanCell(_X-1, _Y);
                if(principalMatrix[_X][_Y+1].isHidden)
                    vCleanCell(_X, _Y+1);
                if(principalMatrix[_X-1][_Y+1].isHidden)
                    vCleanCell(_X-1, _Y+1);
            }
            else if(_X == iX-1 &&
                    _Y == iY-1)
            {
                if(principalMatrix[_X-1][_Y].isHidden)
                    vCleanCell(_X-1, _Y);
                if(principalMatrix[_X][_Y-1].isHidden)
                    vCleanCell(_X, _Y-1);
                if(principalMatrix[_X-1][_Y-1].isHidden)
                    vCleanCell(_X-1, _Y-1);
            }
            else if(_X == 0 &&
                    _Y > 0 &&
                    _Y < iY-1)
            {
                if(principalMatrix[_X+1][_Y].isHidden)
                    vCleanCell(_X+1, _Y);
                if(principalMatrix[_X][_Y+1].isHidden)
                    vCleanCell(_X, _Y+1);
                if(principalMatrix[_X+1][_Y+1].isHidden)
                    vCleanCell(_X+1, _Y+1);
                if(principalMatrix[_X][_Y-1].isHidden)
                    vCleanCell(_X, _Y-1);
                if(principalMatrix[_X+1][_Y-1].isHidden)
                    vCleanCell(_X+1, _Y-1);
            }
            else if(_X == iX-1 &&
                    _Y > 0 &&
                    _Y < iY-1)
            {
                if(principalMatrix[_X-1][_Y].isHidden)
                    vCleanCell(_X-1, _Y);
                if(principalMatrix[_X][_Y+1].isHidden)
                    vCleanCell(_X, _Y+1);
                if(principalMatrix[_X-1][_Y+1].isHidden)
                    vCleanCell(_X-1, _Y+1);
                if(principalMatrix[_X][_Y-1].isHidden)
                    vCleanCell(_X, _Y-1);
                if(principalMatrix[_X-1][_Y-1].isHidden)
                    vCleanCell(_X-1, _Y-1);
            }
            else if(_X > 0 &&
                    _X < iX-1 &&
                    _Y == 0)
            {
                if(principalMatrix[_X-1][_Y].isHidden)
                    vCleanCell(_X-1, _Y);
                if(principalMatrix[_X+1][_Y].isHidden)
                    vCleanCell(_X+1, _Y);
                if(principalMatrix[_X-1][_Y+1].isHidden)
                    vCleanCell(_X-1, _Y+1);
                if(principalMatrix[_X][_Y+1].isHidden)
                    vCleanCell(_X, _Y+1);
                if(principalMatrix[_X+1][_Y+1].isHidden)
                    vCleanCell(_X+1, _Y+1);
            }
            else if(_X > 0 &&
                    _X < iX-1 &&
                    _Y == iY-1)
            {
                if(principalMatrix[_X+1][_Y].isHidden)
                    vCleanCell(_X+1, _Y);
                if(principalMatrix[_X-1][_Y].isHidden)
                    vCleanCell(_X-1, _Y);
                if(principalMatrix[_X-1][_Y-1].isHidden)
                    vCleanCell(_X-1, _Y-1);
                if(principalMatrix[_X][_Y-1].isHidden)
                    vCleanCell(_X, _Y-1);
                if(principalMatrix[_X+1][_Y-1].isHidden)
                    vCleanCell(_X+1, _Y-1);
            }
            else
            {
                if(principalMatrix[_X-1][_Y-1].isHidden)
                    vCleanCell(_X-1, _Y-1);
                if(principalMatrix[_X-1][_Y].isHidden)
                    vCleanCell(_X-1, _Y);
                if(principalMatrix[_X-1][_Y+1].isHidden)
                    vCleanCell(_X-1, _Y+1);
                if(principalMatrix[_X][_Y-1].isHidden)
                    vCleanCell(_X, _Y-1);
                if(principalMatrix[_X][_Y+1].isHidden)
                    vCleanCell(_X, _Y+1);
                if(principalMatrix[_X+1][_Y-1].isHidden)
                    vCleanCell(_X+1, _Y-1);
                if(principalMatrix[_X+1][_Y].isHidden)
                    vCleanCell(_X+1, _Y);
                if(principalMatrix[_X+1][_Y+1].isHidden)
                    vCleanCell(_X+1, _Y+1);
            }
        }
        iHiddenCells--;
        if(iHiddenCells == 0)
        {
            vGameWon();
        }
    }
}

void LibreMinesGameEngine::vGameLost(const uchar _X, const uchar _Y)
{
    qint64 timeInNs = elapsedPreciseTimeInGame.nsecsElapsed();
    bGameActive = false;

    timerTimeInGame.reset();
    emit SIGNAL_gameLost(_X, _Y);
    vGenerateEndGameStatics(timeInNs);
}

void LibreMinesGameEngine::vGameWon()
{
    qint64 timeInNs = elapsedPreciseTimeInGame.nsecsElapsed();
    bGameActive = false;

    timerTimeInGame.reset();
    emit SIGNAL_gameWon();
    vGenerateEndGameStatics(timeInNs);
}


void LibreMinesGameEngine::vGenerateEndGameStatics(qint64 iTimeInNs)
{

    int iCorrectFlags = 0,
            iWrongFlags = 0,
            iUnlockedCells = iCellsToUnlock - iHiddenCells;

    for (int i=0; i<iX; i++)
    {
        for (int j=0; j<iY; j++)
        {
            if(principalMatrix[i][j].hasFlag)
            {
                if (principalMatrix[i][j].state == MINE)
                    iCorrectFlags++;
                else
                    iWrongFlags++;
            }
        }
    }

    double timeInSecs = (double)iTimeInNs*1e-9;

    double dFlagsPerSecond = (double)iCorrectFlags/timeInSecs,
            dCellsPerSecond = (double)iUnlockedCells/timeInSecs,
            dPercentageGameComplete = (double)100*iUnlockedCells/iCellsToUnlock;

    QString QS_Statics =
            "Total time: " + QString::number(timeInSecs, 'f', 3) + " secs"
            +"\nCorrect Flags: " + QString::number(iCorrectFlags)
            +"\nWrongFlags: " + QString::number(iWrongFlags)
            +"\nUnlocked Cells: " + QString::number(iUnlockedCells)
            +"\n"
            +"\nFlags/s: " + QString::number(dFlagsPerSecond, 'f', 3)
            +"\nCells/s: " + QString::number(dCellsPerSecond, 'f', 3)
            +"\n"
            +"\nGame Complete: " + QString::number(dPercentageGameComplete, 'f', 2) + " %";

    emit SIGNAL_endGameStatics(QS_Statics);
}

const std::vector<std::vector<LibreMinesGameEngine::CellGameEngine> >& LibreMinesGameEngine::getPrincipalMatrix() const
{
    return principalMatrix;
}

uchar LibreMinesGameEngine::rows() const
{
    return iX;
}

uchar LibreMinesGameEngine::lines() const
{
    return iY;
}

ushort LibreMinesGameEngine::mines() const
{
    return nMines;
}

bool LibreMinesGameEngine::isGameActive() const
{
    return bGameActive;
}

void LibreMinesGameEngine::setFirstCellClean(const bool x)
{
    bFirstCellClean = x;
}


void LibreMinesGameEngine::SLOT_cleanCell(const uchar _X, const uchar _Y)
{
    if(bFirst && bFirstCellClean && principalMatrix[_X][_Y].state != ZERO)
    {
        vNewGame(iX, iY, nMines, _X, _Y);
        SLOT_startTimer();
        bFirst = false;
    }
    else if(bFirst)
    {
        SLOT_startTimer();
        bFirst = false;
    }
    vCleanCell(_X, _Y);
}

void LibreMinesGameEngine::SLOT_addOrRemoveFlag(const uchar _X, const uchar _Y)
{
    if(!principalMatrix[_X][_Y].isHidden)
        return;

    if(principalMatrix[_X][_Y].hasFlag)
    {
        principalMatrix[_X][_Y].hasFlag = false;
        iMinesLeft++;
        emit SIGNAL_unflagCell(_X, _Y);
    }
    else
    {
        principalMatrix[_X][_Y].hasFlag = true;
        iMinesLeft--;
        emit SIGNAL_flagCell(_X, _Y);
    }

    emit SIGNAL_minesLeft(iMinesLeft);
}

void LibreMinesGameEngine::SLOT_stop()
{
    timerTimeInGame.reset();
    bGameActive = false;
    vResetPrincipalMatrix();
}

void LibreMinesGameEngine::SLOT_startTimer()
{
    iTimeInSeconds = 0;
    timerTimeInGame.reset(new QTimer());
    timerTimeInGame->start(1000);
    QObject::connect(timerTimeInGame.get(), &QTimer::timeout,
                     this, &LibreMinesGameEngine::SLOT_UpdateTime);

    elapsedPreciseTimeInGame.start();
}

void LibreMinesGameEngine::SLOT_UpdateTime()
{
    iTimeInSeconds++;
    emit SIGNAL_currentTime(iTimeInSeconds);
}

