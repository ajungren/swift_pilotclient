/* Copyright (C) 2013
 * swift Project Community / Contributors
 *
 * This file is part of swift project. It is subject to the license terms in the LICENSE file found in the top-level
 * directory of this distribution. No part of swift project, including this file, may be copied, modified, propagated,
 * or distributed except according to the terms contained in the LICENSE file.
 */

//! \cond PRIVATE_TESTS
//! \file
//! \ingroup testblackmisc

#include "blackmisc/math/mathutils.h"
#include "blackmisc/pq/acceleration.h"
#include "blackmisc/pq/angle.h"
#include "blackmisc/pq/constants.h"
#include "blackmisc/pq/frequency.h"
#include "blackmisc/pq/length.h"
#include "blackmisc/pq/mass.h"
#include "blackmisc/pq/measurementunit.h"
#include "blackmisc/pq/physicalquantity.h"
#include "blackmisc/pq/pqstring.h"
#include "blackmisc/pq/pressure.h"
#include "blackmisc/pq/speed.h"
#include "blackmisc/pq/temperature.h"
#include "blackmisc/pq/time.h"
#include "blackmisc/pq/units.h"
#include "blackmisc/variant.h"
#include "test.h"

#include <QString>
#include <QtGlobal>
#include <QTest>

using namespace BlackMisc::PhysicalQuantities;
using namespace BlackMisc::Math;

namespace BlackMiscTest
{
    //! Physical quantities, basic tests
    class CTestPhysicalQuantities : public QObject
    {
        Q_OBJECT

    private slots:
        //! Basic unit tests for physical units
        void unitsBasics();

        //! Basic tests around length
        void lengthBasics();

        //! Basic tests about speed
        void speedBasics();

        //! Frequency tests
        void frequencyTests();

        //! Testing angles (degrees / radians)
        void angleTests();

        //! Testing mass
        void massTests();

        //! Testing pressure
        void pressureTests();

        //! Testing temperature
        void temperatureTests();

        //! Testing time
        void timeTests();

        //! Testing acceleration
        void accelerationTests();

        //! Testing construction / destruction in memory
        void memoryTests();

        //! Test parsing on PQs
        void parserTests();

        //! Basic arithmetic such as +/-
        void basicArithmetic();
    };

    void CTestPhysicalQuantities::unitsBasics()
    {
        // some tests on units
        CLengthUnit du1 = CLengthUnit::m(); // Copy
        CLengthUnit du2 = CLengthUnit::m(); // Copy
        QVERIFY2(du1 == du2, "Compare by value 1");
        du1 = CLengthUnit::m(); // Copy
        du2 = CLengthUnit::m(); // Copy
        QVERIFY2(du1 == du2, "Compare by value 2");
        QVERIFY2(CLengthUnit::m() == CLengthUnit::m(), "Compare by value");

        CFrequencyUnit fu1 = CFrequencyUnit::Hz();
        QVERIFY2(fu1 != du1, "Hz must not be meter");

        // null comparisons
        const CLength null(CLength::null());
        const CLength nonNull(1, CLengthUnit::m());
        QVERIFY2(null == CLength::null(), "null is equal to null");
        QVERIFY2(!(null < CLength::null()), "null is equivalent to null");
        QVERIFY2(null != nonNull, "null is not equal to non-null");
        QVERIFY2((null < nonNull) != (null > nonNull), "null is ordered wrt non-null");
    }

    void CTestPhysicalQuantities::lengthBasics()
    {
        CLength d1(1, CLengthUnit::m()); // 1m
        CLength d2(100, CLengthUnit::cm());
        CLength d3(1.852 * 1000, CLengthUnit::m()); // 1852m
        CLength d4(1, CLengthUnit::NM());
        QVERIFY2(d1 == d2, "1meter shall be 100cm");
        QVERIFY2(d3 == d4, "1852meters shall be 1NM");
        QVERIFY2(d1 * 2 == 2 * d1, "Commutative multiplication");

        d3 *= 2; // SI value
        d4 *= 2.0; // SI value !
        QVERIFY2(d3 == d4, "2*1852meters shall be 2NM");

        // less / greater
        QVERIFY2(!(d1 < d2), "Nothing shall be less / greater");
        QVERIFY2(!(d1 > d2), "Nothing shall be less / greater");

        // epsilon tests
        d1 = d2; // both in same unit
        d1.addValueSameUnit(d1.getUnit().getEpsilon() / 2.0); // this should be still the same
        QVERIFY2(d1 == d2, "Epsilon: 100cm + epsilon shall be 100cm");
        QVERIFY2(!(d1 != d2), "Epsilon: 100cm + epsilon shall be still 100cm");
        QVERIFY2(!(d1 > d2), "d1 shall not be greater");

        d1.addValueSameUnit(d1.getUnit().getEpsilon()); // now over epsilon threshold
        QVERIFY2(d1 != d2, "Epsilon exceeded: 100 cm + 2 epsilon shall not be 100cm");
        QVERIFY2(d1 > d2, "d1 shall be greater");
    }

    void CTestPhysicalQuantities::speedBasics()
    {
        CSpeed s1(100, CSpeedUnit::km_h());
        CSpeed s2(1000, CSpeedUnit::ft_min());
        QVERIFY2(CMathUtils::epsilonEqual(s1.valueRounded(CSpeedUnit::NM_h(), 0), 54), qPrintable(QStringLiteral("100km/h is not %1 NM/h").arg(s1.valueRounded(CSpeedUnit::NM_h(), 0))));
        QVERIFY2(CMathUtils::epsilonEqual(s2.valueRounded(CSpeedUnit::m_s(), 1), 5.1), qPrintable(QStringLiteral("1000ft/min is not %1 m/s").arg(s2.valueRounded(CSpeedUnit::m_s(), 1))));
    }

    void CTestPhysicalQuantities::frequencyTests()
    {
        CFrequency f1(1, CFrequencyUnit::MHz());
        QCOMPARE(f1.valueRounded(CFrequencyUnit::kHz(), 2), 1000.0); // "Mega is 1000kHz
        QCOMPARE(f1.value(), 1.0); // 1MHz
        QCOMPARE(f1.value(CFrequencyUnit::defaultUnit()), 1000000.0); // 1E6 Hz
        CFrequency f2(1e+6, CFrequencyUnit::Hz()) ; // 1 Megahertz
        QVERIFY2(f1 == f2 , "MHz is 1E6 Hz");
    }

    void CTestPhysicalQuantities::angleTests()
    {
        CAngle a1(180, CAngleUnit::deg());
        CAngle a2(1.5 * CAngle::PI(), CAngleUnit::rad());
        CAngle a3(35.4336, CAngleUnit::sexagesimalDeg()); // 35.72666
        CAngle a4(35.436, CAngleUnit::sexagesimalDegMin()); // 35.72666
        CAngle a5(-60.3015, CAngleUnit::sexagesimalDeg()); // negative angles = west longitude or south latitude
        a2.switchUnit(CAngleUnit::deg());
        QVERIFY2(CMathUtils::epsilonEqual(a1.piFactor(), 1.00), qPrintable(QStringLiteral("Pi should be 1PI, not %1").arg(a1.piFactor())));
        QVERIFY2(CMathUtils::epsilonEqual(a3.valueRounded(CAngleUnit::deg()), 35.73), "Expecting 35.73");
        QVERIFY2(CMathUtils::epsilonEqual(a4.valueRounded(CAngleUnit::deg()), 35.73), "Expecting 35.73");
        QVERIFY2(CMathUtils::epsilonEqual(a5.valueRounded(CAngleUnit::deg(), 4), -60.5042), "Expecting -60.5042");

        // issue #552
        CAngle a6(48.07063588, CAngleUnit::sexagesimalDeg());
        CAngle a7(a6);
        a7.switchUnit(CAngleUnit::rad());
        QVERIFY2(CMathUtils::epsilonEqual(a6.value(CAngleUnit::sexagesimalDeg()), a7.value(CAngleUnit::sexagesimalDeg())), "Conversion via radians yields same answer");
        CAngle a8(48.07063588, CAngleUnit::sexagesimalDegMin());
        CAngle a9(a8);
        a9.switchUnit(CAngleUnit::rad());
        QVERIFY2(CMathUtils::epsilonEqual(a8.value(CAngleUnit::sexagesimalDegMin()), a9.value(CAngleUnit::sexagesimalDegMin())), "Conversion via radians yields same answer");
    }

    void CTestPhysicalQuantities::massTests()
    {
        CMass w1(1000, CMassUnit::kg());
        CMass w2(w1.value(), CMassUnit::kg());
        w2.switchUnit(CMassUnit::tonne());
        QCOMPARE(w2.value(), 1.0); // 1tonne shall be 1000kg
        w2.switchUnit(CMassUnit::lb());
        QVERIFY2(CMathUtils::epsilonEqual(w2.valueRounded(2), 2204.62), "1tonne shall be 2204pounds");
        QVERIFY2(w1 == w2, "Masses shall be equal");
    }

    void CTestPhysicalQuantities::pressureTests()
    {
        const CPressure p1(CPhysicalQuantitiesConstants::ISASeaLevelPressure());
        const CPressure p2(29.92, CPressureUnit::inHg());
        CPressure p4(p1);
        p4.switchUnit(CPressureUnit::mbar());

        // does not match exactly
        QVERIFY2(p1 != p2, "Standard pressure test little difference");
        QCOMPARE(p1.value(), p4.value()); // mbar/hPa test

        // Unit substract test
        const CPressure seaLevelPressure(918.0, CPressureUnit::mbar());
        const CPressure standardPressure(1013.2, CPressureUnit::mbar());
        const CPressure delta = (standardPressure - seaLevelPressure);
        const double expected = 95.2;
        const double deltaV = delta.value(CPressureUnit::mbar());
        QCOMPARE(deltaV, expected);
    }

    void CTestPhysicalQuantities::temperatureTests()
    {
        CTemperature t1(0, CTemperatureUnit::C()); // 0C
        CTemperature t2(1, CTemperatureUnit::F()); // 1F
        CTemperature t3(220.15, CTemperatureUnit::F());
        CTemperature t4(10, CTemperatureUnit::F());
        QVERIFY2(CMathUtils::epsilonEqual(t1.valueRounded(CTemperatureUnit::K()), 273.15), qPrintable(QStringLiteral("0C shall be 273.15K, not %1 K").arg(t1.valueRounded(CTemperatureUnit::K()))));
        QVERIFY2(CMathUtils::epsilonEqual(t2.valueRounded(CTemperatureUnit::C()), -17.22), qPrintable(QStringLiteral("1F shall be -17.22C, not %1 C").arg(t2.valueRounded(CTemperatureUnit::C()))));
        QVERIFY2(CMathUtils::epsilonEqual(t3.valueRounded(CTemperatureUnit::C()), 104.53), qPrintable(QStringLiteral("220.15F shall be 104.53C, not %1 C").arg(t3.valueRounded(CTemperatureUnit::C()))));
        QVERIFY2(CMathUtils::epsilonEqual(t4.valueRounded(CTemperatureUnit::K()), 260.93), qPrintable(QStringLiteral("10F shall be 260.93K, not %1 K").arg(t4.valueRounded(CTemperatureUnit::K()))));
    }

    void CTestPhysicalQuantities::timeTests()
    {
        CTime t1(1, CTimeUnit::h());
        CTime t2(1.5, CTimeUnit::h());
        CTime t3(1.25, CTimeUnit::min());
        CTime t4(1.0101, CTimeUnit::hms());
        CTime t5(26, 35, 40);
        CTime t6(3661, CTimeUnit::s());
        CTime t7;
        QVERIFY2(CMathUtils::epsilonEqual(t1.value(CTimeUnit::defaultUnit()), 3600), "1hour shall be 3600s");
        QVERIFY2(CMathUtils::epsilonEqual(t2.value(CTimeUnit::hrmin()), 1.3), "1.5hour shall be 1h30m");
        QVERIFY2(CMathUtils::epsilonEqual(t3.value(CTimeUnit::minsec()), 1.15), "1.25min shall be 1m15s");
        QVERIFY2(CMathUtils::epsilonEqual(t4.value(CTimeUnit::s()), 3661), "1h01m01s shall be 3661s");
        QVERIFY2(CMathUtils::epsilonEqual(t5.value(CTimeUnit::s()), 95740), "Time greater than 24h failed");
        QVERIFY2(t6.formattedHrsMinSec() == "01:01:01", "Formatted output hh:mm:ss failed");
        QVERIFY2(t6.formattedHrsMin() == "01:01", "Formatted output hh:mm failed");
        t6.switchUnit(CTimeUnit::hms());
        QVERIFY2(CMathUtils::epsilonEqual(t6.value(), 1.0101), "Switching the unit produced a wrong a value");
        t7.parseFromString("27:30:55");
        QVERIFY2(t7.formattedHrsMinSec() == "27:30:55", "Parsed time greater than 24h failed");
    }

    void CTestPhysicalQuantities::accelerationTests()
    {
        CLength oneMeter(1, CLengthUnit::m());
        double ftFactor = oneMeter.switchUnit(CLengthUnit::ft()).value();

        CAcceleration a1(10.0, CAccelerationUnit::m_s2());
        CAcceleration a2(a1);
        a1.switchUnit(CAccelerationUnit::ft_s2());
        QVERIFY2(a1 == a2, "Accelerations should be similar");
        QVERIFY2(CMathUtils::epsilonEqual(BlackMisc::Math::CMathUtils::round(a2.value() * ftFactor, 6),
                                          a1.valueRounded(6)), "Numerical values should be equal");
    }

    void CTestPhysicalQuantities::memoryTests()
    {
        CLength *c = new CLength(100, CLengthUnit::m());
        c->switchUnit(CLengthUnit::NM());
        QVERIFY2(c->getUnit() == CLengthUnit::NM() && CLengthUnit::defaultUnit() == CLengthUnit::m(),
                 "Testing distance units failed");
        delete c;

        CAngle *a = new CAngle(100, CAngleUnit::rad());
        a->switchUnit(CAngleUnit::deg());
        QVERIFY2(a->getUnit() == CAngleUnit::deg() && CAngleUnit::defaultUnit() == CAngleUnit::deg(),
                 "Testing angle units failed");
        delete a;
    }

    void CTestPhysicalQuantities::parserTests()
    {
        QVERIFY2(CLength(33.0, CLengthUnit::ft()) == CLength("33.0 ft"), "Length");
        QVERIFY2(CLength(33.0, CLengthUnit::ft()) != CLength("33.1 ft"), "Length !=");
        QVERIFY2(CLength(-22.8, CLengthUnit::ft()) != CLength("-22.8 cm"), "Length !=");
        QVERIFY2(CSpeed(123.45, CSpeedUnit::km_h()) == CSpeed("123.45km/h"), "Speed");
        QVERIFY2(CMass(33.45, CMassUnit::kg()) == CMass("33.45000 kg"), "CMass");


        // parsing via variant
        CSpeed parsedPq1 = CPqString::parseToVariant("100.123 km/h").value<CSpeed>();
        QVERIFY2(CSpeed(100.123, CSpeedUnit::km_h()) == parsedPq1, "Parsed speed via variant");

        CLength parsedPq2 = CPqString::parseToVariant("-33.123ft").value<CLength>();
        QVERIFY2(CLength(-33.123, CLengthUnit::ft()) == parsedPq2, "Parsed length via variant");

        CFrequency parsedPq3 = CPqString::parse<CFrequency>("122.8MHz");
        QVERIFY2(CFrequency(122.8, CFrequencyUnit::MHz()) == parsedPq3, "Parsed frequency via variant");
    }

    void CTestPhysicalQuantities::basicArithmetic()
    {
        // pressure
        CPressure p1 = CPhysicalQuantitiesConstants::ISASeaLevelPressure();
        CPressure p2(p1);
        p2 *= 2.0;
        CPressure p3 = p1 + p1;
        QVERIFY2(p3 == p2, "Pressure needs to be the same (2times)");
        p3 /= 2.0;
        QVERIFY2(p3 == p1, "Pressure needs to be the same (1time)");
        p3 = p3 - p3;
        QCOMPARE(p3.value() + 1, 1.0); // Value needs to be zero
        p3 = CPressure(1013, CPressureUnit::hPa());
        QVERIFY2(p3 * 1.5 == 1.5 * p3, "Basic commutative test on PQ failed");

        // the time clasas
        CTime time1;
        time1.parseFromString("11:30"); // hhmm

        CTime time2;
        time2.parseFromString("-11:30"); // hhmm

        CTime time3 = time1 + time2;
        QVERIFY2(time3.isZeroEpsilonConsidered(), "Time must be 0");

        // angle
        CAngle a1(180, CAngleUnit::deg());
        CAngle a2(1.5 * CAngle::PI(), CAngleUnit::rad());
        a1 += a2;
        QVERIFY2(a1.valueInteger(CAngleUnit::deg()) == 450, "Expect 450 degrees");

    }
} // namespace

//! main
BLACKTEST_APPLESS_MAIN(BlackMiscTest::CTestPhysicalQuantities);

#include "testphysicalquantities.moc"

//! \endcond
