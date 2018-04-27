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
	Three controls are connected with constraints.

	Some constant distances use a unit suffix for the
	wrong physical quantity, while others don't have a
	unit at all. Both will result in compile-time
	error messages.
-->
<gui-model>
	<controls>
		<control>
			<sides>
				<left>10</left>
				<bottom>10</bottom>
				<right>272.0833</right>
				<top>247.2222</top>
			</sides>
			<borderSize>2</borderSize>
			<name>Screen</name>
			<nameHeight>10</nameHeight>
		</control>
		<control>
			<sides>
				<left>55.71146</left>
				<bottom>202.4944</bottom>
				<right>112.4808</right>
				<top>155.575</top>
			</sides>
			<borderSize>2</borderSize>
			<name>Alice</name>
			<nameHeight>10</nameHeight>
		</control>
		<control>
			<sides>
				<left>155.851</left>
				<bottom>205.6694</bottom>
				<right>225.3141</right>
				<top>117.1222</top>
			</sides>
			<borderSize>2</borderSize>
			<name>Bob</name>
			<nameHeight>10</nameHeight>
		</control>
		<control>
			<sides>
				<left>71.22604</left>
				<bottom>105.4806</bottom>
				<right>122.7063</right>
				<top>44.45</top>
			</sides>
			<borderSize>2</borderSize>
			<name>Malory</name>
			<nameHeight>10</nameHeight>
		</control>
	</controls>
	<constraints>
		<constraint>
			<text>5mm</text>
			<text-height>7</text-height>
			<first-end-point>
				<control>2</control>
				<side>left</side>
			</first-end-point>
			<second-end-point>
				<control>1</control>
				<side>right</side>
			</second-end-point>
			<first-local-side>164.5986</first-local-side>
			<second-local-side>171.5986</second-local-side>
		</constraint>
		<constraint>
			<text>5kg</text>
			<text-height>7</text-height>
			<first-end-point>
				<control>1</control>
				<side>top</side>
			</first-end-point>
			<second-end-point>
				<control>3</control>
				<side>bottom</side>
			</second-end-point>
			<first-local-side>90.20456</first-local-side>
			<second-local-side>97.20456</second-local-side>
		</constraint>
		<constraint>
			<text>42</text>
			<text-height>7</text-height>
			<first-end-point>
				<control>2</control>
				<side>top</side>
			</first-end-point>
			<second-end-point>
				<control>3</control>
				<side>top</side>
			</second-end-point>
			<first-local-side>217.8473</first-local-side>
			<second-local-side>224.8473</second-local-side>
		</constraint>
	</constraints>
</gui-model>
