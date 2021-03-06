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
	Two controls of equal height and proportional width are
	placed next to each other so that they leave gaps of
	constant size to the left and right and between them.
	Their height is 1/4 of the window height, as is their
	distance from window's top.

	This traditionally very hard to do with just HTML and
	CSS (no JavaScript).
-->
<gui-model>
	<controls>
		<control>
			<sides>
				<left>10</left>
				<bottom>10</bottom>
				<right>201.6667</right>
				<top>176.8519</top>
			</sides>
			<borderSize>2</borderSize>
			<name>Screen</name>
			<nameHeight>10</nameHeight>
		</control>
		<control>
			<sides>
				<left>21.96041</left>
				<bottom>155.976</bottom>
				<right>92.86873</right>
				<top>107.25</top>
			</sides>
			<borderSize>2</borderSize>
			<name>name</name>
			<nameHeight>10</nameHeight>
		</control>
		<control>
			<sides>
				<left>107.1562</left>
				<bottom>155.9759</bottom>
				<right>177.0063</right>
				<top>108.0444</top>
			</sides>
			<borderSize>2</borderSize>
			<name>type</name>
			<nameHeight>10</nameHeight>
		</control>
	</controls>
	<constraints>
		<constraint>
			<text>5px</text>
			<text-height>7</text-height>
			<first-end-point>
				<control>2</control>
				<side>left</side>
			</first-end-point>
			<second-end-point>
				<control>1</control>
				<side>right</side>
			</second-end-point>
			<first-local-side>111.7607</first-local-side>
			<second-local-side>118.7606</second-local-side>
		</constraint>
		<constraint>
			<text>a</text>
			<text-height>7</text-height>
			<first-end-point>
				<control>0</control>
				<side>top</side>
			</first-end-point>
			<second-end-point>
				<control>1</control>
				<side>bottom</side>
			</second-end-point>
			<first-local-side>47.96145</first-local-side>
			<second-local-side>54.96145</second-local-side>
		</constraint>
		<constraint>
			<text>2a</text>
			<text-height>7</text-height>
			<first-end-point>
				<control>0</control>
				<side>bottom</side>
			</first-end-point>
			<second-end-point>
				<control>1</control>
				<side>top</side>
			</second-end-point>
			<first-local-side>78.12395</first-local-side>
			<second-local-side>85.12395</second-local-side>
		</constraint>
		<constraint>
			<text>2a</text>
			<text-height>7</text-height>
			<first-end-point>
				<control>0</control>
				<side>bottom</side>
			</first-end-point>
			<second-end-point>
				<control>2</control>
				<side>top</side>
			</second-end-point>
			<first-local-side>120.4573</first-local-side>
			<second-local-side>127.4573</second-local-side>
		</constraint>
		<constraint>
			<text>5px</text>
			<text-height>7</text-height>
			<first-end-point>
				<control>1</control>
				<side>left</side>
			</first-end-point>
			<second-end-point>
				<control>0</control>
				<side>left</side>
			</second-end-point>
			<first-local-side>111.0056</first-local-side>
			<second-local-side>118.0056</second-local-side>
		</constraint>
		<constraint>
			<text>5px</text>
			<text-height>7</text-height>
			<first-end-point>
				<control>0</control>
				<side>right</side>
			</first-end-point>
			<second-end-point>
				<control>2</control>
				<side>right</side>
			</second-end-point>
			<first-local-side>111.3366</first-local-side>
			<second-local-side>118.3366</second-local-side>
		</constraint>
		<constraint>
			<text>b</text>
			<text-height>7</text-height>
			<first-end-point>
				<control>1</control>
				<side>right</side>
			</first-end-point>
			<second-end-point>
				<control>1</control>
				<side>left</side>
			</second-end-point>
			<first-local-side>111.4296</first-local-side>
			<second-local-side>118.4296</second-local-side>
		</constraint>
		<constraint>
			<text>2b</text>
			<text-height>7</text-height>
			<first-end-point>
				<control>2</control>
				<side>right</side>
			</first-end-point>
			<second-end-point>
				<control>2</control>
				<side>left</side>
			</second-end-point>
			<first-local-side>111.562</first-local-side>
			<second-local-side>118.562</second-local-side>
		</constraint>
		<constraint>
			<text>a</text>
			<text-height>7</text-height>
			<first-end-point>
				<control>2</control>
				<side>top</side>
			</first-end-point>
			<second-end-point>
				<control>2</control>
				<side>bottom</side>
			</second-end-point>
			<first-local-side>207.6375</first-local-side>
			<second-local-side>214.6375</second-local-side>
		</constraint>
		<constraint>
			<text>a</text>
			<text-height>7</text-height>
			<first-end-point>
				<control>0</control>
				<side>top</side>
			</first-end-point>
			<second-end-point>
				<control>2</control>
				<side>bottom</side>
			</second-end-point>
			<first-local-side>120.4573</first-local-side>
			<second-local-side>127.4573</second-local-side>
		</constraint>
	</constraints>
</gui-model>
