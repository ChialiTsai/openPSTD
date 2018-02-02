//////////////////////////////////////////////////////////////////////////
// This file is part of openPSTD.                                       //
//                                                                      //
// openPSTD is free software: you can redistribute it and/or modify     //
// it under the terms of the GNU General Public License as published by //
// the Free Software Foundation, either version 3 of the License, or    //
// (at your option) any later version.                                  //
//                                                                      //
// openPSTD is distributed in the hope that it will be useful,          //
// but WITHOUT ANY WARRANTY; without even the implied warranty of       //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        //
// GNU General Public License for more details.                         //
//                                                                      //
// You should have received a copy of the GNU General Public License    //
// along with openPSTD.  If not, see <http://www.gnu.org/licenses/>.    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// Date: 10-4-2016
//
//
// Authors: M. R. Fortuin
//
//
// Purpose:
//
//
//////////////////////////////////////////////////////////////////////////

#ifndef OPENPSTD_MOUSESELECTSTRATEGY_H
#define OPENPSTD_MOUSESELECTSTRATEGY_H

#include <memory>
#include <QMouseEvent>
#include "GUI/operations/BaseOperation.h"
#include "GUI/Model.h"
#include "MouseStrategy.h"

namespace OpenPSTD
{
    namespace GUI
    {
        class MouseSelectStrategy : public MouseStrategy
        {
        public:
            virtual void mouseReleaseEvent(std::shared_ptr <Model> const &model, QMouseEvent *mouseEvent,
                                           QVector2D pos) override;
        };

    }
}


#endif //OPENPSTD_MOUSESELECTSTRATEGY_H
