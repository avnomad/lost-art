<?xml version="1.0" encoding="utf-8"?>
<!--
	Copyright (C) 2014, 2018 Vaptistis Anogeianakis <nomad@cornercase.gr>

	This file is part of LostArt.

	LostArt is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	LostArt is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with LostArt.  If not, see <http://www.gnu.org/licenses/>.
-->

<!--
	A single rectangle is centered horizontally and vertically and
	spans exactly half the window's width and height.

	The same set of constraints applies on each axis, and they
	are expressed in two different but equivalent ways.
-->
<gui-model>
	<controls>
		<control>
			<sides>
				<left>62.89063</left>
				<bottom>82.31945</bottom>
				<right>226.5974</right>
				<top>221.4694</top>
			</sides>
			<borderSize>2</borderSize>
			<name>Screen</name>
			<nameHeight>10</nameHeight>
		</control>
		<control>
			<sides>
				<left>106.1339</left>
				<bottom>185.5611</bottom>
				<right>179.8281</right>
				<top>119.5917</top>
			</sides>
			<borderSize>2</borderSize>
			<name>Closure</name>
			<nameHeight>10</nameHeight>
		</control>
	</controls>
	<constraints>
		<constraint>
			<text>a</text>
			<text-height>7</text-height>
			<first-end-point>
				<control>1</control>
				<side>left</side>
			</first-end-point>
			<second-end-point>
				<control>0</control>
				<side>left</side>
			</second-end-point>
			<first-local-side>60.89375</first-local-side>
			<second-local-side>67.89375</second-local-side>
		</constraint>
		<constraint>
			<text>2a</text>
			<text-height>7</text-height>
			<first-end-point>
				<control>1</control>
				<side>right</side>
			</first-end-point>
			<second-end-point>
				<control>1</control>
				<side>left</side>
			</second-end-point>
			<first-local-side>61.5875</first-local-side>
			<second-local-side>68.5875</second-local-side>
		</constraint>
		<constraint>
			<text>a</text>
			<text-height>7</text-height>
			<first-end-point>
				<control>0</control>
				<side>right</side>
			</first-end-point>
			<second-end-point>
				<control>1</control>
				<side>right</side>
			</second-end-point>
			<first-local-side>60.89375</first-local-side>
			<second-local-side>67.89375</second-local-side>
		</constraint>
		<constraint>
			<text>1/2b</text>
			<text-height>7</text-height>
			<first-end-point>
				<control>1</control>
				<side>bottom</side>
			</first-end-point>
			<second-end-point>
				<control>0</control>
				<side>top</side>
			</second-end-point>
			<first-local-side>238.739</first-local-side>
			<second-local-side>245.739</second-local-side>
		</constraint>
		<constraint>
			<text>b</text>
			<text-height>7</text-height>
			<first-end-point>
				<control>1</control>
				<side>top</side>
			</first-end-point>
			<second-end-point>
				<control>1</control>
				<side>bottom</side>
			</second-end-point>
			<first-local-side>238.5628</first-local-side>
			<second-local-side>245.5628</second-local-side>
		</constraint>
		<constraint>
			<text>1/2b</text>
			<text-height>7</text-height>
			<first-end-point>
				<control>0</control>
				<side>bottom</side>
			</first-end-point>
			<second-end-point>
				<control>1</control>
				<side>top</side>
			</second-end-point>
			<first-local-side>238.3864</first-local-side>
			<second-local-side>245.3864</second-local-side>
		</constraint>
	</constraints>
</gui-model>
