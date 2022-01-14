/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <infos@oslandia.com>
 *   Copyright (C) 2012-2013 IGN (http://www.ign.fr)
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Library General Public
 *   License as published by the Free Software Foundation; either
 *   version 2 of the License, or (at your option) any later version.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Library General Public License for more details.

 *   You should have received a copy of the GNU Library General Public
 *   License along with this library; if not, see
 <http://www.gnu.org/licenses/>.
 */
#include <boost/test/unit_test.hpp>

#include <SFCGAL/GeometryCollection.h>
#include <SFCGAL/Kernel.h>
#include <SFCGAL/LineString.h>
#include <SFCGAL/MultiLineString.h>
#include <SFCGAL/MultiPoint.h>
#include <SFCGAL/MultiPolygon.h>
#include <SFCGAL/MultiSolid.h>
#include <SFCGAL/Point.h>
#include <SFCGAL/Polygon.h>
#include <SFCGAL/PolyhedralSurface.h>
#include <SFCGAL/Solid.h>
#include <SFCGAL/Triangle.h>
#include <SFCGAL/TriangulatedSurface.h>
#include <SFCGAL/algorithm/area.h>
#include <SFCGAL/algorithm/covers.h>
#include <SFCGAL/algorithm/offset.h>
#include <SFCGAL/io/wkt.h>

#include <SFCGAL/detail/tools/Registry.h>

using namespace boost::unit_test;
using namespace SFCGAL;

BOOST_AUTO_TEST_SUITE(SFCGAL_algorithm_OffsetTest)

BOOST_AUTO_TEST_CASE(testEmpty)
{
  tools::Registry         &registry = tools::Registry::instance();
  std::vector<std::string> typeNames =
      tools::Registry::instance().getGeometryTypes();

  for (auto &typeName : typeNames) {
    std::unique_ptr<Geometry> g(registry.newGeometryByTypeName(typeName));
    BOOST_CHECK(algorithm::offset(*g, 1.0)->isEmpty());
  }
}

BOOST_AUTO_TEST_CASE(testSimple)
{
  std::unique_ptr<Geometry> gA(
      io::readWkt("POLYGON((0 0,10 0,10 10,0 10,0 0))"));

  std::unique_ptr<MultiPolygon> result(algorithm::offset(*gA, 1.0));
  BOOST_CHECK_EQUAL(
      result->asText(7),
      "MULTIPOLYGON(((-1.0000000 10.0000000,-1.0000000 0.0000000,-0.8750000 "
      "-0.4841229,-0.7500000 -0.6614378,-0.6250000 -0.7806247,-0.5000000 "
      "-0.8660254,-0.3750000 -0.9270248,-0.2500000 -0.9682458,-0.1250000 "
      "-0.9921567,0.0000000 -1.0000000,10.0000000 -1.0000000,10.1250000 "
      "-0.9921567,10.2500000 -0.9682458,10.3750000 -0.9270248,10.5000000 "
      "-0.8660254,10.6250000 -0.7806247,10.7500000 -0.6614378,10.8750000 "
      "-0.4841229,11.0000000 0.0000000,11.0000000 10.0000000,10.8750000 "
      "10.4841229,10.7500000 10.6614378,10.6250000 10.7806247,10.5000000 "
      "10.8660254,10.3750000 10.9270248,10.2500000 10.9682458,10.1250000 "
      "10.9921567,10.0000000 11.0000000,0.0000000 11.0000000,-0.1250000 "
      "10.9921567,-0.2500000 10.9682458,-0.3750000 10.9270248,-0.5000000 "
      "10.8660254,-0.6250000 10.7806247,-0.7500000 10.6614378,-0.8750000 "
      "10.4841229,-1.0000000 10.0000000)))");
}

BOOST_AUTO_TEST_CASE(testHoles)
{
  std::unique_ptr<Geometry> gA(
      io::readWkt("POLYGON((13.652901 8.978070,13.921068 13.219992,20.454603 "
                  "13.268750,18.967492 11.001516,16.432091 11.220926,15.091253 "
                  "13.024961,14.481782 11.976670,14.676813 10.708970,15.798240 "
                  "9.392511,16.358954 9.416890,16.870910 10.075119,16.797774 "
                  "10.952758,17.431624 11.001516,18.162990 10.099498,18.138611 "
                  "8.368599,14.774328 9.416890,14.530540 8.929313,13.945447 "
                  "8.441735,13.652901 8.978070))"));

  std::unique_ptr<MultiPolygon> result(algorithm::offset(*gA, 1.0));
  BOOST_REQUIRE(result->is<MultiPolygon>());

  // @todo works in Release, not in Debug... Something strange with holes?
  // BOOST_CHECK_EQUAL(result->asText(),
  // "MULTIPOLYGON(((3562035796054625/281474976710656
  // 2544860917657577/281474976710656,7123931396669903/562949953421312
  // 317964373056855/35184372088832,1780947800307639/140737488355328
  // 2542483596591303/281474976710656,890456375723901/70368744177664
  // 2541144291051935/281474976710656,7123510810351861/562949953421312
  // 634915648506851/70368744177664,3561685307456257/281474976710656
  // 2537980476987837/281474976710656,3561615209736583/281474976710656
  // 5071969601116369/562949953421312,7123090224033819/562949953421312
  // 2533383492583149/281474976710656,890368753574309/70368744177664
  // 2527102044156639/281474976710656,7131540527014219/562949953421312
  // 4956233456451353/562949953421312,3570065512716983/281474976710656
  // 2458093175000837/281474976710656,446795095240857/35184372088832
  // 4885823703043269/562949953421312,7157312022273459/562949953421312
  // 2430267883251895/281474976710656,3582951260346603/281474976710656
  // 1209632512805569/140737488355328,896811627389119/70368744177664
  // 4818883948181671/562949953421312,7183083517532699/562949953421312
  // 4801047510081963/562949953421312,3595837007976223/281474976710656
  // 2392330622366059/281474976710656,3617296621970919/281474976710656
  // 1176491447249259/140737488355328,3678191227646117/281474976710656
  // 4482695395763713/562949953421312,7463205603800977/562949953421312
  // 8687610570303929/1125899906842624,946253594038715/70368744177664
  // 4264218238232021/562949953421312,7676851900818463/562949953421312
  // 2108402143761123/281474976710656,3891837524663603/281474976710656
  // 4193315285240291/562949953421312,7890498197835949/562949953421312
  // 8381481625982275/1125899906842624,1999330336586173/140737488355328
  // 2104391789045301/281474976710656,8104144494853435/562949953421312
  // 8499318072011689/1125899906842624,4105483821681089/281474976710656
  // 1079948365930079/140737488355328,4135117370846867/281474976710656
  // 2184589861506093/281474976710656,4270180842118197/281474976710656
  // 1148572194737795/140737488355328,4272612752338525/281474976710656
  // 4598388807690023/562949953421312,4275044662558853/281474976710656
  // 2301292113989641/281474976710656,8554953145558363/562949953421312
  // 4606877899262883/562949953421312,8559816965999019/562949953421312
  // 576409108444147/70368744177664,8564680786439675/562949953421312
  // 4615772382578285/562949953421312,2142386151720083/140737488355328
  // 2310189958518351/281474976710656,2143602106830247/140737488355328
  // 1156274797071269/140737488355328,2144818061940411/140737488355328
  // 4629934181399431/562949953421312,1172300896754761/70368744177664
  // 4380928336815057/562949953421312,2510914993244079/140737488355328
  // 4173639518782967/562949953421312,2533738879155213/140737488355328
  // 1038332500998499/140737488355328,2556562765066347/140737488355328
  // 1037088893370329/140737488355328,2579386650977481/140737488355328
  // 8316604178249921/1125899906842624,2602210536888615/140737488355328
  // 8368027159152935/1125899906842624,2625034422799749/140737488355328
  // 4228000636828651/562949953421312,2647858308710883/140737488355328
  // 8592066857742173/1125899906842624,2670682194622017/140737488355328
  // 8807363562616145/1125899906842624,2693506080533151/140737488355328
  // 587896613523929/70368744177664,2695197738703343/140737488355328
  // 2591758803655759/281474976710656,5393617379598531/281474976710656
  // 5641102357406905/562949953421312,5416194808534473/281474976710656
  // 5652009239260505/562949953421312,5438772237470415/281474976710656
  // 5667000427346193/562949953421312,5461349666406357/281474976710656
  // 5686438188185139/562949953421312,2741963547671149/140737488355328
  // 2855429841689847/281474976710656,172078266383695/8796093022208
  // 717633948950611/70368744177664,2764540976607091/140737488355328
  // 1444584179193095/140737488355328,1387914845537531/70368744177664
  // 2912394300792355/281474976710656,2787118405543033/140737488355328
  // 91946033833445/8796093022208,2834370595143243/140737488355328
  // 3086352725700583/281474976710656,749102727878519/35184372088832
  // 895110862135765/70368744177664,1499646457529951/70368744177664
  // 7179222032924185/562949953421312,93817966206429/4398046511104
  // 7199286657112375/562949953421312,6010113844303109/281474976710656
  // 7221499951581983/562949953421312,6015877851394761/281474976710656
  // 7246502689655235/562949953421312,6021641858486413/281474976710656
  // 3637684448267739/281474976710656,3013702932789033/140737488355328
  // 3655091554796859/281474976710656,3016584936334859/140737488355328
  // 919537476873557/70368744177664,3019466939880685/140737488355328
  // 3734821097229517/281474976710656,3001743472325599/140737488355328
  // 7743126126422099/562949953421312,5968040009541027/281474976710656
  // 122549782625263/8796093022208,741574134303857/35184372088832
  // 3955175776166125/281474976710656,1474286534830171/70368744177664
  // 1989594465864045/140737488355328,366356200263157/17592186044416
  // 7992565217965855/562949953421312,5826252269100341/281474976710656
  // 8015519965766225/562949953421312,2895402666995085/140737488355328
  // 4014313812801613/281474976710656,2877679199439999/140737488355328
  // 1004072059083869/70368744177664,2414491598236189/140737488355328
  // 4009374978246145/281474976710656,3916331769260527/281474976710656
  // 2001282039644627/140737488355328,3881480060680763/281474976710656
  // 1000033954465311/70368744177664,3846628352100999/281474976710656
  // 3993259312549527/281474976710656,3811776643521235/281474976710656
  // 3981582516415721/281474976710656,3776924934941471/281474976710656
  // 3964415313379727/281474976710656,3742073226361707/281474976710656
  // 3940472515079713/281474976710656,3707221517781943/281474976710656
  // 3907156532257937/281474976710656,3672369809202179/281474976710656
  // 7715553844946187/562949953421312,3637518100622415/281474976710656
  // 233678492800729/17592186044416,3597395696223959/281474976710656
  // 3104192447845677/281474976710656,3562035796054625/281474976710656
  // 2544860917657577/281474976710656)))" ) ;

  // Check area
  BOOST_CHECK_CLOSE(algorithm::area(*result), 47.7, 1.0);
}

BOOST_AUTO_TEST_CASE(testPoint)
{
  std::unique_ptr<Geometry>     gA(io::readWkt("POINT(1 1)"));
  std::unique_ptr<MultiPolygon> result(algorithm::offset(*gA, 1.0));
  BOOST_CHECK_EQUAL(
      result->asText(2),
      "MULTIPOLYGON(((2.00 1.00,1.75 1.66,1.50 1.87,1.25 1.97,1.00 2.00,0.75 "
      "1.97,0.50 1.87,0.25 1.66,0.00 1.00,0.25 0.34,0.50 0.13,0.75 0.03,1.00 "
      "0.00,1.25 0.03,1.50 0.13,1.75 0.34,2.00 1.00)))");
}

BOOST_AUTO_TEST_CASE(testLineString)
{
  std::unique_ptr<Geometry> gA(
      io::readWkt("LINESTRING(13.652901 8.978070,13.921068 13.219992,20.454603 "
                  "13.268750,18.967492 11.001516,16.432091 11.220926,15.091253 "
                  "13.024961,14.481782 11.976670,14.676813 10.708970,15.798240 "
                  "9.392511,16.358954 9.416890,16.870910 10.075119,16.797774 "
                  "10.952758,17.431624 11.001516,18.162990 10.099498,18.138611 "
                  "8.368599,14.774328 9.416890,14.530540 8.929313,13.945447 "
                  "8.441735,13.652901 8.978070)"));
  std::unique_ptr<MultiPolygon> result(algorithm::offset(*gA, 0.5));
  //	BOOST_CHECK_EQUAL( result->asText(7),
  //"MULTIPOLYGON(((20.9546030 13.2687500,20.8916366 13.5116525,20.8286702 13.6005236,20.7657038
  // 13.6601785,20.7027374 13.7028345,20.6397710 13.7331989,20.5768046 13.7535869,20.5138382
  // 13.7652288,20.4508718 13.7687361,17.1597259 13.7441753,13.9173367 13.7199781,13.8554277
  // 13.7156646,13.7935186 13.7034495,13.7316095 13.6827073,13.6697004 13.6522123,13.6077914
  // 13.6096813,13.5458823 13.5505002,13.4839732 13.4627844,13.4220642 13.2515382,13.2795207
  // 10.9967608,13.1538971 9.0096161,13.1537726 9.0075805,13.1536481 9.0053931,13.1535236
  // 9.0030140,13.1533991 9.0003820,13.1532746 8.9973940,13.1531500 8.9938489,13.1530255
  // 8.9892281,13.1529010 8.9780700,13.1605309 8.8910546,13.1681608 8.8554856,13.1757907
  // 8.8285182,13.1834206 8.8060580,13.1910505 8.7865130,13.1986804 8.7690637,13.2063103
  // 8.7532218,13.2139402 8.7386679,13.2901713 8.5988927,13.5065036 8.2023008,13.6013817
  // 8.0789416,13.6962597 8.0082541,13.7911377 7.9661421,13.8860157 7.9452796,13.9808937
  // 7.9429931,14.0757717 7.9590182,14.1706498 7.9953229,14.2655278 8.0576148,14.3708002
  // 8.1453365,14.8506349 8.5452046,14.8665248 8.5590236,14.8824146 8.5740888,14.8983045
  // 8.5905664,14.9141943 8.6086740,14.9300842 8.6287062,14.9459740 8.6510783,14.9618639
  // 8.6764104,14.9777537 8.7057064,15.0314338 8.8130668,16.8081412 8.2594549,17.9898675
  // 7.8912361,18.0709543 7.8731976,18.1520410 7.8687794,18.2331277 7.8776137,18.3142145
  // 7.9004501,18.3953012 7.9395184,18.4763879 7.9999437,18.5574747 8.0955546,18.6385614
  // 8.3615562,18.6505813 9.2148186,18.6629404 10.0924576,18.6629466 10.0929123,18.6629528
  // 10.0934008,18.6629590 10.0939320,18.6629652 10.0945196,18.6629714 10.0951866,18.6629776
  // 10.0959777,18.6629838 10.0970088,18.6629900 10.0994980,18.6490372 10.2167929,18.6350845
  // 10.2642002,18.6211317 10.2997633,18.6071789 10.3290545,18.5932262 10.3542466,18.5792734
  // 10.3764603,18.5653206 10.3963651,18.5513679 10.4143990,18.4456305 10.5448084,18.9243838
  // 10.5033778,18.9820333 10.5017275,19.0396828 10.5067550,19.0973323 10.5186687,19.1549818
  // 10.5379995,19.2126312 10.5657331,19.2702807 10.6036228,19.3279302 10.6549842,19.3855797
  // 10.7272849,19.7213263 11.2391580,20.8726915 12.9945202,20.8829305 13.0108051,20.8931694
  // 13.0286260,20.9034083 13.0483554,20.9136473 13.0705623,20.9238862 13.0962006,20.9341251
  // 13.1271219,20.9443641 13.1680817,20.9546030 13.2687500),(14.4933005 10.1533469,14.6995207
  // 9.9112622,14.6529699 9.9019387,14.6064191 9.8878535,14.5598683 9.8685614,14.5133175
  // 9.8433563,14.4667667 9.8111057,14.4202159 9.7698838,14.3736651 9.7160040,14.3271142
  // 9.6404965,14.1767673 9.3398020,14.2471573 10.4532437,14.2532865 10.4432158,14.2594156
  // 10.4336893,14.2655448 10.4246137,14.2716740 10.4159471,14.2778032 10.4076543,14.2839324
  // 10.3997051,14.2900615 10.3920736,14.2961907 10.3847375,14.4933005 10.1533469),(16.3330313
  // 10.5088578,16.3564315 10.2280600,16.1062333 9.9063813,16.0446992 9.9037077,16.0204939
  // 9.9026545,15.2545447 10.8018136,15.1494276 10.9252117,15.0586554 11.5152302,15.0028310
  // 11.8780880,15.0656050 11.9860593,15.1423368 12.1180381,16.0307937 10.9226635,16.0662176
  // 10.8801377,16.1016414 10.8456886,16.1370653 10.8172432,16.1724892 10.7936007,16.2079130
  // 10.7739985,16.2433369 10.7579229,16.2787607 10.7450165,16.3141846 10.7350268,16.3330313
  // 10.5088578),(17.1671884 12.7442029,19.5241216 12.7617921,18.8851471 11.7876198,18.7131511
  // 11.5253950,17.8521962 11.5999009,16.6992425 11.6996758,15.9297694 12.7349684,17.1671884
  // 12.7442029),(17.3535812 10.3038685,17.6604872 9.9253511,17.6506763 9.2289018,17.6480888
  // 9.0451535,17.1056283 9.2141811,16.8875736 9.2821256,17.2655882 9.7681514,17.2787535
  // 9.7858722,17.2919187 9.8053938,17.3050839 9.8271411,17.3182491 9.8517634,17.3314143
  // 9.8803480,17.3445796 9.9150030,17.3577448 9.9611370,17.3709100 10.0751190,17.3706941
  // 10.0898101,17.3704782 10.0958931,17.3702624 10.1005593,17.3700465 10.1044917,17.3698306
  // 10.1079552,17.3696147 10.1110853,17.3693989 10.1139629,17.3691830 10.1166403,17.3535812
  // 10.3038685)))" ); Check area
  BOOST_CHECK_CLOSE(algorithm::area(*result), 29.2515, 1.0);
}

BOOST_AUTO_TEST_CASE(testPolygonWithHoles)
{
  std::unique_ptr<Geometry>     gA(io::readWkt(
          "POLYGON((11.966308 -10.211022,18.007885 1.872133,39.364158 "
              "2.434140,53.554839 -6.557975,43.438710 -22.856183,20.396416 "
              "-28.476254,5.643728 -25.525717,13.090323 -20.889158,32.479570 "
              "-21.310663,38.521147 -15.831093,46.248746 -9.087007,34.446595 "
              "-1.359409,22.784946 -14.988082,11.966308 -10.211022),(20.396416 "
              "-1.640412,15.900358 -7.260484,18.007885 -9.508513,22.644444 "
              "-9.368011,25.173477 -2.342921,20.396416 -1.640412),(41.050179 "
              "-0.797401,40.207168 -2.202419,47.934767 -6.557975,48.496774 "
              "-5.433961,41.050179 -0.797401))"));
  std::unique_ptr<MultiPolygon> result(algorithm::offset(*gA, 0.5));

  // @todo works in Release, not in Debug... Something strange with holes?
  // BOOST_CHECK_EQUAL(result->asText(),
  // "MULTIPOLYGON(((3028360385708393/562949953421312
  // -3532689497542891/140737488355328,3011773017246331/562949953421312
  // -1767740319401211/70368744177664,2995185648784269/562949953421312
  // -3538735168852129/140737488355328,5957196560644415/1125899906842624
  // -885635926734687/35184372088832,5924021823720291/1125899906842624
  // -3547045526205937/140737488355328,5890847086796167/1125899906842624
  // -1776237153685471/70368744177664,1464418087468011/281474976710656
  // -7118564940832791/281474976710656,364031100809245/70368744177664
  // -892156430622343/35184372088832,1447830719005949/281474976710656
  // -1796212649524449/70368744177664,5847891174287959/1125899906842624
  // -7246337522204351/281474976710656,2952229736276061/562949953421312
  // -908684373322589/35184372088832,5961027770816285/1125899906842624
  // -3642778888469283/140737488355328,47012469289691/8796093022208
  // -114025703884643/4398046511104,6074164367344611/1125899906842624
  // -7306929440326075/281474976710656,3065366332804387/562949953421312
  // -3657008390538791/140737488355328,6187300963872937/1125899906842624
  // -7319255082343209/281474976710656,1560967315534275/281474976710656
  // -3661427529807983/140737488355328,4044410832301929/281474976710656
  // -3909771877196161/140737488355328,357092489940035/17592186044416
  // -2038339348858059/70368744177664,715137315710917/35184372088832
  // -8154664641645539/281474976710656,5728717212334113/281474976710656
  // -1019443289175517/35184372088832,2868167949490445/140737488355328
  // -1019501301714505/35184372088832,2871977292813833/140737488355328
  // -2039015268623291/70368744177664,2875786636137221/140737488355328
  // -2038924685863837/70368744177664,5759191958921219/281474976710656
  // -8154920204131587/281474976710656,1441702661391999/70368744177664
  // -4076859215539645/140737488355328,1443607333053693/70368744177664
  // -4076041132401399/140737488355328,4124457677447379/140737488355328
  // -3774274582775847/140737488355328,1532532312414221/35184372088832
  // -821271613867985/35184372088832,3067759237626369/70368744177664
  // -3283542150172497/140737488355328,383806731303037/8796093022208
  // -6563029166824747/281474976710656,3073148463222223/70368744177664
  // -6557913080183645/281474976710656,1537921538010075/35184372088832
  // -6551598303243473/281474976710656,3078537688818077/70368744177664
  // -3271939700952919/140737488355328,770308075404001/17592186044416
  // -3267217465268595/140737488355328,3083926914413931/70368744177664
  // -6522718912523963/281474976710656,1543310763605929/35184372088832
  // -3253831650909595/140737488355328,1627423549295689/35184372088832
  // -2711771472227055/140737488355328,1899240410453665/35184372088832
  // -3840251168849529/562949953421312,7598284220876781/140737488355328
  // -7662735592119657/1125899906842624,3799803399969451/70368744177664
  // -3821663758345067/562949953421312,118764521546891/2199023255552
  // -7621877342095167/1125899906842624,7602251958063145/140737488355328
  // -3798886152874089/562949953421312,3801787268562633/70368744177664
  // -7569985056110207/1125899906842624,1901224279046847/35184372088832
  // -7536521012352269/1125899906842624,7606219695249509/140737488355328
  // -7492254852128817/1125899906842624,3803771137155815/70368744177664
  // -3691811720788129/562949953421312,7603454299374331/140737488355328
  // -7194542331184431/1125899906842624,949920790554629/17592186044416
  // -1780063123697661/281474976710656,3797639174749867/70368744177664
  // -883259226368679/140737488355328,7591190374562435/140737488355328
  // -7022829590175691/1125899906842624,474193899976571/8796093022208
  // -3493463818415547/562949953421312,3791507212343919/70368744177664
  // -3478256199439985/562949953421312,7578926449750539/140737488355328
  // -3465238858842713/562949953421312,946854809351655/17592186044416
  // -215878235704859/35184372088832,888843660288911/17592186044416
  // -569436605777293/140737488355328,1394419417996963/35184372088832
  // 804029475622255/281474976710656,5572738170160599/140737488355328
  // 6477933794533037/2251799813685248,2783899334166673/70368744177664
  // 6515605797025573/2251799813685248,2781429583253047/70368744177664
  // 6546103931253125/2251799813685248,5557919664678841/140737488355328
  // 3285015590788249/1125899906842624,1388245040712897/35184372088832
  // 1646953464083803/562949953421312,346752541314021/8796093022208
  // 6599745053018491/2251799813685248,5543101159197083/140737488355328
  // 6606010991358149/2251799813685248,2769080828684915/70368744177664
  // 51614892923215/17592186044416,1997906675328045/70368744177664
  // 2978665338405763/1125899906842624,1266266617940965/70368744177664
  // 1335294738140175/562949953421312,2524897179804049/140737488355328
  // 2665644420170763/1125899906842624,314657640465771/17592186044416
  // 2653862938285587/1125899906842624,5019250135296573/281474976710656
  // 5269573376258579/2251799813685248,5003978023140811/281474976710656
  // 5215162004938759/2251799813685248,4988705910985049/281474976710656
  // 5141629971653071/2251799813685248,2486716899414643/140737488355328
  // 78810939479743/35184372088832,1239540421668381/70368744177664
  // 4911855402350817/2251799813685248,2471444787258881/140737488355328
  // 589898310588529/281474976710656,1856178995772707/140737488355328
  // -935582431948529/140737488355328,1621168410962437/140737488355328
  // -2811207476072225/281474976710656,405059945792677/35184372088832
  // -703766943252719/70368744177664,1619311155378979/140737488355328
  // -88101919123843/8796093022208,6473530110348999/562949953421312
  // -5647743478572963/562949953421312,6469815599182083/562949953421312
  // -5658052605853291/562949953421312,6466101088015167/562949953421312
  // -2834939166027707/281474976710656,3231193288424125/281474976710656
  // -2842026402083069/281474976710656,3229336032840667/281474976710656
  // -1425679275736801/140737488355328,3227478777257209/281474976710656
  // -718536794910499/70368744177664,3237964905494173/281474976710656
  // -2927454063023811/281474976710656,6496902067462275/562949953421312
  // -5896123042034813/562949953421312,1629468580984051/140737488355328
  // -1481416340455219/140737488355328,1634711645102533/140737488355328
  // -2974381847490525/281474976710656,1639954709221015/140737488355328
  // -5967464990266985/562949953421312,6580791093357989/562949953421312
  // -2991411736662575/281474976710656,3300881674915959/281474976710656
  // -2997731537569245/281474976710656,3311367803152923/281474976710656
  // -1501446124617571/140737488355328,2724489330497645/140737488355328
  // -493346525637421/35184372088832,1589135921473041/70368744177664
  // -1086878775772863/70368744177664,6377016448265889/281474976710656
  // -4354725352994157/281474976710656,3198744605319807/140737488355328
  // -8717212281320445/562949953421312,3208980986506669/140737488355328
  // -8718866616362967/562949953421312,6438434735387063/281474976710656
  // -8714523162756849/562949953421312,1614726874440197/70368744177664
  // -8703888656897737/562949953421312,404961266258407/17592186044416
  // -135721204885481/8796093022208,6499853022508237/281474976710656
  // -8659635153660283/562949953421312,3260162892440981/140737488355328
  // -67347968157629/4398046511104,3547258472461965/140737488355328
  // -3639227363839613/281474976710656,1215289358062885/35184372088832
  // -284093325341281/140737488355328,3047410182890251/70368744177664
  // -4367395071093191/562949953421312,1598180599613823/35184372088832
  // -1286903614230609/140737488355328,2780663059477579/70368744177664
  // -4024971450789115/281474976710656,2687549805184669/70368744177664
  // -4350021453112993/281474976710656,5374973997463783/140737488355328
  // -4350241230170015/281474976710656,1343712096139557/35184372088832
  // -8700924124712407/562949953421312,5374722771652673/140737488355328
  // -8701367912610695/562949953421312,2687298579373559/70368744177664
  // -8701813837478525/562949953421312,5374471545841563/140737488355328
  // -8702261912938807/562949953421312,671793241617001/17592186044416
  // -8702712152797035/562949953421312,5374220320030453/140737488355328
  // -8703164571044669/562949953421312,2687047353562449/70368744177664
  // -4351809590931299/281474976710656,582917783571653/17592186044416
  // -705135161401155/35184372088832,2272294256681503/70368744177664
  // -1464123814792323/70368744177664,101183728333231/4398046511104
  // -2899840932584139/140737488355328,3687657146802913/281474976710656
  // -2869535508454603/140737488355328,7355952602553787/562949953421312
  // -5739193651996305/281474976710656,1834147727875437/140737488355328
  // -2869992609691077/140737488355328,1829307305112427/140737488355328
  // -5741457203098911/281474976710656,7297867529397669/562949953421312
  // -5743631671330411/281474976710656,3639252919172815/281474976710656
  // -359158938446617/17592186044416,3629572073646795/281474976710656
  // -1437560227466965/70368744177664,7239782456241551/562949953421312
  // -5754795153095311/281474976710656,902552595648689/70368744177664
  // -2880151818206675/140737488355328,4016516006338149/562949953421312
  // -844718186952545/35184372088832,3028360385708393/562949953421312
  // -3532689497542891/140737488355328),(408817578010145/8796093022208
  // -2682724776110247/562949953421312,3366855130504311/70368744177664
  // -395309996290287/70368744177664,1679087410278771/35184372088832
  // -825340995389983/140737488355328,1625998263599139/35184372088832
  // -1411298169994999/281474976710656,1439049553265513/35184372088832
  // -568329426454073/281474976710656,5800833692634477/140737488355328
  // -839088728978075/562949953421312,408817578010145/8796093022208
  // -2682724776110247/562949953421312),(3245366561247467/140737488355328
  // -2856916395619525/1125899906842624,3447504739790557/140737488355328
  // -386840753297585/140737488355328,809599447857921/35184372088832
  // -483846606191777/70368744177664,196058214955373/8796093022208
  // -2499088320953625/281474976710656,2848639514157999/140737488355328
  // -629140128733581/70368744177664,5127997789525703/281474976710656
  // -5067623122676177/562949953421312,1167992938984399/70368744177664
  // -1023691869936171/140737488355328,2330811895639755/140737488355328
  // -2036345911013337/281474976710656,2411464969609961/140737488355328
  // -229339184794687/35184372088832,2900259490773367/140737488355328
  // -612727014782355/281474976710656,3245366561247467/140737488355328
  // -2856916395619525/1125899906842624)))" );

  BOOST_CHECK_CLOSE(algorithm::area(*result), 696.0, 1.0);
}

BOOST_AUTO_TEST_CASE(testMultiPoint)
{
  std::unique_ptr<Geometry> gA(
      io::readWkt("MULTIPOINT(2 0,1 1,0 2,-1 1,-2 0,-1 -1,0 -2,1 -1)"));
  std::unique_ptr<Geometry> result(
      io::readWkt(algorithm::offset(*gA, 1.0)->asText(7)));
  std::unique_ptr<Geometry> expected(io::readWkt(
      "MULTIPOLYGON(((3.0000000 0.0000000,2.8750000 0.4841229,2.7500000 "
      "0.6614378,2.6250000 0.7806247,2.5000000 0.8660254,2.3750000 "
      "0.9270248,2.2500000 0.9682458,2.1250000 0.9921567,2.0000000 "
      "1.0000000,1.8750000 1.4841229,1.7500000 1.6614378,1.6250000 "
      "1.7806247,1.5000000 1.8660254,1.3750000 1.9270248,1.2500000 "
      "1.9682458,1.1250000 1.9921567,1.0000000 2.0000000,0.7500000 "
      "2.6614378,0.5000000 2.8660254,0.2500000 2.9682458,0.0000000 "
      "3.0000000,-0.2500000 2.9682458,-0.5000000 2.8660254,-0.7500000 "
      "2.6614378,-1.0000000 2.0000000,-1.1250000 1.9921567,-1.2500000 "
      "1.9682458,-1.3750000 1.9270248,-1.5000000 1.8660254,-1.6250000 "
      "1.7806247,-1.7500000 1.6614378,-1.8750000 1.4841229,-2.0000000 "
      "1.0000000,-2.1250000 0.9921567,-2.2500000 0.9682458,-2.3750000 "
      "0.9270248,-2.5000000 0.8660254,-2.6250000 0.7806247,-2.7500000 "
      "0.6614378,-2.8750000 0.4841229,-3.0000000 0.0000000,-2.8750000 "
      "-0.4841229,-2.7500000 -0.6614378,-2.6250000 -0.7806247,-2.5000000 "
      "-0.8660254,-2.3750000 -0.9270248,-2.2500000 -0.9682458,-2.1250000 "
      "-0.9921567,-2.0000000 -1.0000000,-1.8750000 -1.4841229,-1.7500000 "
      "-1.6614378,-1.6250000 -1.7806247,-1.5000000 -1.8660254,-1.3750000 "
      "-1.9270248,-1.2500000 -1.9682458,-1.1250000 -1.9921567,-1.0000000 "
      "-2.0000000,-0.7500000 -2.6614378,-0.5000000 -2.8660254,-0.2500000 "
      "-2.9682458,0.0000000 -3.0000000,0.2500000 -2.9682458,0.5000000 "
      "-2.8660254,0.7500000 -2.6614378,1.0000000 -2.0000000,1.1250000 "
      "-1.9921567,1.2500000 -1.9682458,1.3750000 -1.9270248,1.5000000 "
      "-1.8660254,1.6250000 -1.7806247,1.7500000 -1.6614378,1.8750000 "
      "-1.4841229,2.0000000 -1.0000000,2.1250000 -0.9921567,2.2500000 "
      "-0.9682458,2.3750000 -0.9270248,2.5000000 -0.8660254,2.6250000 "
      "-0.7806247,2.7500000 -0.6614378,2.8750000 -0.4841229,3.0000000 "
      "0.0000000),(0.0000000 1.0000000,0.1250000 0.5158771,0.2500000 "
      "0.3385622,0.3750000 0.2193753,0.5000000 0.1339746,0.6250000 "
      "0.0729752,0.7500000 0.0317542,0.8750000 0.0078433,1.0000000 "
      "0.0000000,0.8750000 -0.0078433,0.7500000 -0.0317542,0.6250000 "
      "-0.0729752,0.5000000 -0.1339746,0.3750000 -0.2193753,0.2500000 "
      "-0.3385622,0.1250000 -0.5158771,0.0000000 -1.0000000,-0.1250000 "
      "-0.5158771,-0.2500000 -0.3385622,-0.3750000 -0.2193753,-0.5000000 "
      "-0.1339746,-0.6250000 -0.0729752,-0.7500000 -0.0317542,-0.8750000 "
      "-0.0078433,-1.0000000 0.0000000,-0.8750000 0.0078433,-0.7500000 "
      "0.0317542,-0.6250000 0.0729752,-0.5000000 0.1339746,-0.3750000 "
      "0.2193753,-0.2500000 0.3385622,-0.1250000 0.5158771,0.0000000 "
      "1.0000000)))"));
  // results for clang64 on mingw differ. Prefer covers.
  BOOST_CHECK(algorithm::covers(*result, *expected));
}

BOOST_AUTO_TEST_CASE(testMultiLineString)
{
  std::unique_ptr<Geometry> gA(
      io::readWkt("MULTILINESTRING((0.0 0.0,1.0 0.0),(2.0 2.0,3.0 2.0))"));
  std::unique_ptr<Geometry> result{algorithm::offset(*gA, 1.0)};
  std::unique_ptr<Geometry> expected{io::readWkt(
      "MULTIPOLYGON(((2/1 0/1,15/8 4360591588697965/9007199254740992,7/4 "
      "2978851154656373/4503599627370496,13/8 "
      "3515621332314565/4503599627370496,3/2 "
      "3900231685776981/4503599627370496,11/8 "
      "1043737148218427/1125899906842624,5/4 "
      "4360591588697965/4503599627370496,9/8 "
      "8936553463969119/9007199254740992,1/1 1/1,0/1 1/1,-1/8 "
      "8936553463969119/9007199254740992,-1/4 "
      "4360591588697965/4503599627370496,-3/8 "
      "1043737148218427/1125899906842624,-1/2 "
      "3900231685776981/4503599627370496,-5/8 "
      "3515621332314565/4503599627370496,-3/4 "
      "2978851154656373/4503599627370496,-7/8 "
      "4360591588697965/9007199254740992,-1/1 0/1,-7/8 "
      "-4360591588697965/9007199254740992,-3/4 "
      "-2978851154656373/4503599627370496,-5/8 "
      "-3515621332314565/4503599627370496,-1/2 "
      "-3900231685776981/4503599627370496,-3/8 "
      "-1043737148218427/1125899906842624,-1/4 "
      "-4360591588697965/4503599627370496,-1/8 "
      "-8936553463969119/9007199254740992,0/1 -1/1,1/1 -1/1,9/8 "
      "-8936553463969119/9007199254740992,5/4 "
      "-4360591588697965/4503599627370496,11/8 "
      "-1043737148218427/1125899906842624,3/2 "
      "-3900231685776981/4503599627370496,13/8 "
      "-3515621332314565/4503599627370496,7/4 "
      "-2978851154656373/4503599627370496,15/8 "
      "-4360591588697965/9007199254740992,2/1 0/1)),((4/1 2/1,31/8 "
      "5593747524544987/2251799813685248,15/4 "
      "2996512602349341/1125899906842624,29/8 "
      "3130705146763889/1125899906842624,7/2 "
      "3226857735129493/1125899906842624,27/8 "
      "3295536961903675/1125899906842624,13/4 "
      "3341947710859739/1125899906842624,25/8 "
      "842217249170347/281474976710656,3/1 3/1,2/1 3/1,15/8 "
      "842217249170347/281474976710656,7/4 "
      "3341947710859739/1125899906842624,13/8 "
      "3295536961903675/1125899906842624,3/2 "
      "3226857735129493/1125899906842624,11/8 "
      "3130705146763889/1125899906842624,5/4 "
      "2996512602349341/1125899906842624,9/8 "
      "5593747524544987/2251799813685248,1/1 2/1,9/8 "
      "3413451730196005/2251799813685248,5/4 "
      "6028348100084619/4503599627370496,11/8 "
      "5491577922426427/4503599627370496,3/2 "
      "5106967568964011/4503599627370496,13/8 "
      "1208062665466821/1125899906842624,7/4 "
      "4646607666043027/4503599627370496,15/8 "
      "283682657672277/281474976710656,2/1 1/1,3/1 1/1,25/8 "
      "283682657672277/281474976710656,13/4 "
      "4646607666043027/4503599627370496,27/8 "
      "1208062665466821/1125899906842624,7/2 "
      "5106967568964011/4503599627370496,29/8 "
      "5491577922426427/4503599627370496,15/4 "
      "6028348100084619/4503599627370496,31/8 "
      "3413451730196005/2251799813685248,4/1 2/1)))")};
  // results for clang64 on mingw differ. Prefer almostEqual.
  BOOST_CHECK(result->almostEqual(*expected, 0.1));
}

BOOST_AUTO_TEST_CASE(testMultiPolygon)
{
  std::unique_ptr<Geometry> gA(
      io::readWkt("MULTIPOLYGON(((0 0,1 0,1 1,0 0)),((2 1,2 0,3 0,2 1)))"));
  std::unique_ptr<Geometry> result{algorithm::offset(*gA, 1.0)};
  std::unique_ptr<Geometry> expected{io::readWkt(
      "MULTIPOLYGON(((-3184560061929027/4503599627370496 "
      "1592245805114451/2251799813685248,-6698880015218421/9007199254740992 "
      "1505295754916495/2251799813685248,-1757159976644697/2251799813685248 "
      "2816374419278037/4503599627370496,-1839599949484789/2251799813685248 "
      "5194573209446149/9007199254740992,-7688159689299523/9007199254740992 "
      "4692743228266015/9007199254740992,-4008959790329945/4503599627370496 "
      "8207948346006429/18014398509481984,-4173839736010129/4503599627370496 "
      "3383176886746873/9007199254740992,-8677439363380625/9007199254740992 "
      "150930505912737/562949953421312,-1/1 0/1,-7/8 "
      "-4360591588697965/9007199254740992,-3/4 "
      "-2978851154656373/4503599627370496,-5/8 "
      "-3515621332314565/4503599627370496,-1/2 "
      "-3900231685776981/4503599627370496,-3/8 "
      "-1043737148218427/1125899906842624,-1/4 "
      "-4360591588697965/4503599627370496,-1/8 "
      "-8936553463969119/9007199254740992,0/1 -1/1,1/1 -1/1,17/16 "
      "-8989589855190815/9007199254740992,9/8 "
      "-8936553463969119/9007199254740992,19/16 "
      "-8847452998693645/9007199254740992,5/4 "
      "-4360591588697965/4503599627370496,21/16 "
      "-4278049268158919/4503599627370496,11/8 "
      "-1043737148218427/1125899906842624,23/16 "
      "-4049719698998033/4503599627370496,3/2 "
      "-3900231685776981/4503599627370496,25/16 "
      "-4049719698998033/4503599627370496,13/8 "
      "-1043737148218427/1125899906842624,27/16 "
      "-4278049268158919/4503599627370496,7/4 "
      "-4360591588697965/4503599627370496,29/16 "
      "-8847452998693645/9007199254740992,15/8 "
      "-8936553463969119/9007199254740992,31/16 "
      "-8989589855190815/9007199254740992,2/1 -1/1,3/1 -1/1,25/8 "
      "-8936553463969119/9007199254740992,13/4 "
      "-4360591588697965/4503599627370496,27/8 "
      "-1043737148218427/1125899906842624,7/2 "
      "-3900231685776981/4503599627370496,29/8 "
      "-3515621332314565/4503599627370496,15/4 "
      "-2978851154656373/4503599627370496,31/8 "
      "-4360591588697965/9007199254740992,4/1 "
      "0/1,2231189820475225/562949953421312 "
      "4829776189207591/18014398509481984,1105289913632601/281474976710656 "
      "6766353773493757/18014398509481984,8759879336220717/2251799813685248 "
      "2051987086501605/4503599627370496,8677439363380625/2251799813685248 "
      "4692743228266013/9007199254740992,8594999390540533/2251799813685248 "
      "5194573209446149/9007199254740992,4256279708850221/1125899906842624 "
      "2816374419278035/4503599627370496,4215059722430175/1125899906842624 "
      "752647877458247/1125899906842624,4173839736010129/1125899906842624 "
      "1592245805114451/2251799813685248,2004476136010603/562949953421312 "
      "3844055643426829/4503599627370496,761982451173185/281474976710656 "
      "480508208490967/281474976710656,5756089623278279/2251799813685248 "
      "4123128542440257/2251799813685248,2708159818585539/1125899906842624 "
      "538791190997139/281474976710656,5076549651063877/2251799813685248 "
      "1107372277498785/562949953421312,1184194916239169/562949953421312 "
      "35089795868257/17592186044416,4397009678849475/2251799813685248 "
      "2250537735633209/1125899906842624,2028619846371137/1125899906842624 "
      "2229458379429329/1125899906842624,3717469706635073/2251799813685248 "
      "2180959401449141/1125899906842624,3/2 "
      "8403831313147477/4503599627370496,3037929734420671/2251799813685248 "
      "2180959401449141/1125899906842624,1349079874156735/1125899906842624 "
      "2229458379429329/1125899906842624,2358389762206269/2251799813685248 "
      "2250537735633209/1125899906842624,504654944024767/562949953421312 "
      "35089795868257/17592186044416,1678849789991867/2251799813685248 "
      "1107372277498785/562949953421312,669539901942333/1125899906842624 "
      "538791190997139/281474976710656,999309817777465/2251799813685248 "
      "4123128542440257/2251799813685248,2638159326681055/9007199254740992 "
      "480508208490967/281474976710656,-315626275746667/562949953421312 "
      "3844055643426829/4503599627370496,-3184560061929027/4503599627370496 "
      "1592245805114451/2251799813685248)))")};
  // results for clang64 on mingw differ. Prefer almostEqual.
  BOOST_CHECK(result->almostEqual(*expected, 0.1));
}

BOOST_AUTO_TEST_CASE(testGeometryCollection)
{
  std::unique_ptr<Geometry> gA(
      io::readWkt("GEOMETRYCOLLECTION(POINT(0 0),LINESTRING(1 1,2 2))"));
  std::unique_ptr<Geometry> result(
      io::readWkt(algorithm::offset(*gA, 1.0)->asText(2)));
  std::unique_ptr<Geometry> expected(io::readWkt(
      "MULTIPOLYGON(((-0.00 1.00,-0.12 0.99,-0.25 0.97,-0.38 0.93,-0.50 "
      "0.87,-0.62 0.78,-0.75 0.66,-0.88 0.48,-1.00 0.00,-0.75 -0.66,-0.50 "
      "-0.87,-0.25 -0.97,0.00 -1.00,0.25 -0.97,0.50 -0.87,0.75 -0.66,1.00 "
      "0.00,1.09 0.00,1.18 0.02,1.27 0.04,1.35 0.06,1.44 0.10,1.53 0.15,1.62 "
      "0.21,1.71 0.29,1.85 0.44,2.71 1.29,2.74 1.33,2.78 1.37,2.82 1.42,2.85 "
      "1.48,2.89 1.54,2.93 1.62,2.96 1.73,3.00 2.00,2.79 2.62,2.57 2.82,2.36 "
      "2.93,2.15 2.99,1.93 3.00,1.72 2.96,1.51 2.87,1.29 2.71,0.44 1.85,0.29 "
      "1.71,0.26 1.67,0.22 1.63,0.18 1.58,0.15 1.52,0.11 1.46,0.07 1.38,0.04 "
      "1.27,-0.00 1.00)))"));
  BOOST_CHECK(algorithm::covers(*result, *expected));
}

BOOST_AUTO_TEST_SUITE_END()
